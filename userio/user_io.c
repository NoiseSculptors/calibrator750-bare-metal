
// user_io.c (strong overrides)
#include "dma.h"
#include "dmamux.h"
#include "gpio.h"
#include "i2c.h"
#include "printf.h"
#include "rcc.h"
#include "serial.h"
#include "ssd1315.h"
#include "syscall.h"
#include "timer.h"
#include "user_io.h"
#include "user_io_config.h"
#include <string.h>

extern clock_info_t ci;

void user_btn_init(void) {
/* buttons  0:E3 1:E15 */
    *RCC_AHB4ENR |= (0x1u<<GPIOEEN);

    gpio_ctrl(GPIOE, GPIO_MODE, GPIO3|GPIO15, MODE_INPUT);
    gpio_ctrl(GPIOE, GPIO_PUPD, GPIO3|GPIO15, PUPD_PULLUP);
}

void user_dipswitch_init(void) {
/* dip switch 0: 0:B4 1:B3 2:D7 3:D6 4:D5 5:D4
   dip switch 1: 0:B0 1:C5 2:C4 3:A7 4:A6 5:A5 */
    /* dipswitch 0|1 */
    *RCC_AHB4ENR |= (0x1u<<GPIOAEN)|(0x1u<<GPIOBEN)|
                    (0x1u<<GPIOCEN)|(0x1u<<GPIODEN);

    /* dipswitch 0: */

    gpio_ctrl(GPIOB, GPIO_MODE, GPIO3|GPIO4, MODE_INPUT);
    gpio_ctrl(GPIOB, GPIO_PUPD, GPIO3|GPIO4, PUPD_PULLUP);

    gpio_ctrl(GPIOD, GPIO_MODE, GPIO4|GPIO5|GPIO6|GPIO7, MODE_INPUT);
    gpio_ctrl(GPIOD, GPIO_PUPD, GPIO4|GPIO5|GPIO6|GPIO7, PUPD_PULLUP);

    /* dipswitch 1: */

    gpio_ctrl(GPIOA, GPIO_MODE, GPIO5|GPIO6|GPIO7, MODE_INPUT);
    gpio_ctrl(GPIOA, GPIO_PUPD, GPIO5|GPIO6|GPIO7, PUPD_PULLUP);

    gpio_ctrl(GPIOB, GPIO_MODE, GPIO0, MODE_INPUT);
    gpio_ctrl(GPIOB, GPIO_PUPD, GPIO0, PUPD_PULLUP);

    gpio_ctrl(GPIOC, GPIO_MODE, GPIO4|GPIO5, MODE_INPUT);
    gpio_ctrl(GPIOC, GPIO_PUPD, GPIO4|GPIO5, PUPD_PULLUP);
}
void user_led_init(void) {
/* leds D0 D1 C10 */
    *RCC_AHB4ENR |= (0x1u<<GPIOCEN)|(0x1u<<GPIODEN);

    gpio_ctrl(GPIOC, GPIO_MODE, GPIO10, MODE_OUTPUT);
    gpio_ctrl(GPIOD, GPIO_MODE, GPIO0|GPIO1, MODE_OUTPUT);
}

void user_enc_init(void) {
/* encoder  (B6 AF2 TIM4_CH1) (B7 AF2 TIM4_CH2) */
    *RCC_AHB4ENR |= (0x1u<<GPIOBEN);

    gpio_ctrl(GPIOB, GPIO_MODE, GPIO6|GPIO7, MODE_AF);

/* rest of encoder configuration */
#define TIM4EN 2
    *RCC_APB1LENR |= (1<<TIM4EN);

    /* AF mode outputs AF2 for PA6 and PA7, AF1 for PA8 and PA9 */

    gpio_ctrl(GPIOB, GPIO_AFRL, GPIO6|GPIO7, AF2);

#define CC4E 12 
#define CC3E 8
#define CC2E 4
#define CC1E 0
/* CC1E: Capture/Compare 1 output enable.
    0: Capture mode disabled / OC1 is not active
    1: Capture mode enabled / OC1 signal is output on the corresponding output pin */
    *TIM4_CCER &= ~((1<<CC2E)|(1<<CC1E));

#define SMS 0
#define CC2S 8
#define CC1S 0 
    // Configure TIM3 and TIM1 for Encoder Input Mode
    *TIM4_SMCR |= (0x3u << SMS);  // Encoder mode 3: Counts on both TI1 and TI2
    *TIM4_CCMR1 &= ~0xFFFF;

    // Set channels as input mapped to TI1 and TI2
    *TIM4_CCMR1 |= (0x2u << CC2S) | (0x2u << CC1S);
    *TIM4_ARR = 0xFFFF;

    /* Enabe timers */
#define CEN 0
    *TIM4_CR1 |= (1<<CEN);
    *TIM4_CNT = 1;
}

void user_serial_init(void) {
    init_usart1_pa10_pa9(&ci,115200);
}

/********** Display ***********************************************************/

/*
 * The framebuffer is kept free of I2C control bytes to make it easy to use.
 *
 * DMA cannot transfer the full framebuffer in one shot (limit ~254 bytes),
 * and every I2C data transfer requires at least a leading 0x40 control byte.
 * Embedding 0x40 into the framebuffer would make drawing logic awkward,
 * especially in non-page addressing modes.
 *
 * Using page addressing mode allows each page to be sent with a small
 * 3-byte header followed by raw pixel data.
 */

#define OLED_WIDTH     128
#define OLED_HEIGHT    64
#define OLED_ROWS 8
#define OLED_ADDR 0x3C
#define HDR_SIZE  3
#define STRIDE_SIZE    (HDR_SIZE + OLED_WIDTH)
#define I2CPERIPH I2C3

uint8_t oled_buf[OLED_ROWS * STRIDE_SIZE];
extern uint8_t font_8x8_linux[256*8];
uint8_t *fb_128_64[1024]; // contiguous pointer filled, for convenience

static void init_dma_oled(void){
    uint8_t dma_request_mux_input = 74;
    *RCC_AHB1ENR |= (1u<<0); // DMA1EN
    *DMAMUX1_C6CR = 0; // clear first
    *DMAMUX1_C6CR = (dma_request_mux_input & 0x7F);
    *DMA1_S6CR &= ~1u;
    while (*DMA1_S6CR & 1u) {}
    *DMA1_S6PAR  = (uint32_t)I2C_TXDR(I2CPERIPH);
    *DMA1_S6CR = (1u<<10) // MINC=1
               | (1u<<6); // DIR = Memory to peripheral
    *I2C_CR1(I2CPERIPH) |= (1u<<14)  // TXDMAEN
                     | (1u<<0);   // enable

    /* set page addressing mode */
    i2c_write_bytes(I2CPERIPH, OLED_ADDR, (uint8_t[]){
               0x00, 0x20, 0x02, // Set Page Addressing Mode (0x00/0x01/0x02)
               0x00, 0x00,       // lower column = 0
               0x00, 0x10,       // higher column = 0
           }, 7);

    for (uint8_t row = 0; row < OLED_ROWS; row++)
    {
        uint8_t *b = &oled_buf[row * STRIDE_SIZE];

// NOTE:
// The control byte for commands *must* use the continuation bit (0x80)
// if command bytes and the following data bytes are sent in the same
// I2C transaction (same START..STOP frame).

        // Minimal header per row
        b[0] = 0x80;                   // cmd control 
        b[1] = 0xB0 | (row & 0x0F);    // page address
        b[2] = 0x40;                   // DATA

     /* init with every other row turned on for debugging purposes */
        memset(&b[HDR_SIZE], 0x00, OLED_WIDTH);
    }
}

static void oled_init(void) {
    init_i2c3_pa8_pc9(&ci);
    init_ssd1315(I2CPERIPH);
    int i=0;
    uint8_t *p;
    p = oled_buf;
    for(int row=0; row<OLED_ROWS; row++){
        p+=HDR_SIZE;
        for(int column = 0; column<OLED_WIDTH; column++){
           fb_128_64[i] = p+i;
           i++;
        }
    }
    init_dma_oled();
    ssd1315_set_contrast(1);
}

static void oled_systick_dma(void) {

    /* non-blocking - dma is triggered only if last
       i2c transaction is finished transfered */
    if (*DMA1_S6CR & 1u)
        return;

    static uint8_t row = 0;
    uint8_t* row_ptr = &oled_buf[row * STRIDE_SIZE];


    *DMA1_HIFCR = (1u<<21)|(1u<<19);
    *DMA1_S6M0AR = (uint32_t)row_ptr;
    *DMA1_S6NDTR = STRIDE_SIZE;
    *DMA1_S6CR |= (1<<0);
    *I2C_CR2(I2CPERIPH) = ((OLED_ADDR<<1)&0x3FF)
                     | (STRIDE_SIZE<<16)
                     | (1u<<25)   // AUTOEND
                     | (1u<<13);  // START

    if (row++ >= OLED_ROWS)
        row = 0;
}

void user_display_init(void) {
    oled_init();
}

void user_display_set_pixel(size_t i, int x, int y, uint16_t color)
{
    switch(i){
        case 0:
            if ((unsigned)x >= OLED_WIDTH || (unsigned)y >= OLED_HEIGHT)
                return;

            uint8_t page = (uint8_t)(y >> 3);
            uint8_t bit  = (uint8_t)(y & 7);

            uint8_t *row = &oled_buf[page * STRIDE_SIZE + HDR_SIZE];
            if(color==0)
                row[x] &= (uint8_t)~(1u << bit);
            else
                row[x] |= (uint8_t)(1u << bit);
        break;
    }
}

size_t user_display_count(void) { return USER_NUM_DISPLAYS; }


/* 8 rows * 14 characters + null byte */
#define PRINTF_BUFSZ (8*14)+1

int fb_printf(int x, int y, const char *fmt, ...) {
    char buf[PRINTF_BUFSZ];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if(!buf[0]) return n;
    int col = x, row = y;
    for(const char *p = buf; *p; p++){
        if(*p=='\n' || col>=14){
            col=0; row++;
            if(row>=OLED_ROWS) break;
            if(*p=='\n') continue;
        }
        uint8_t *dst = oled_buf + 1 + row*STRIDE_SIZE + HDR_SIZE + col*9;
        const uint8_t *g = font_8x8_linux + (*p)*8;
        for(int i=0;i<8;i++)
            dst[i] = g[i];
        dst[8] = 0; col++;
    }
    return n;
}

/********** !Display **********************************************************/

size_t user_btn_count(void) { return USER_NUM_BTNS; }
uint8_t user_btn_read(size_t i)
{
    switch(i){
        case 0: return ((*GPIOE_IDR >> IDR3) & 0x1u) == 0;  /* left button */
        case 1: return ((*GPIOE_IDR >> IDR15) & 0x1u) == 0; /* right button */
    }
    return 0;
}

size_t user_led_count(void) { return USER_NUM_LEDS; }
void user_led_write(size_t i, uint8_t on)
{
    switch(i){
        case 0: on==1?*GPIOD_BSRR|=(1<<BS1) : (*GPIOD_BSRR|=(1<<BR1));break;
        case 1: on==1?*GPIOD_BSRR|=(1<<BS0) : (*GPIOD_BSRR|=(1<<BR0));break;
        case 2: on==1?*GPIOC_BSRR|=(1<<BS10): (*GPIOC_BSRR|=(1<<BR10));break;
    }
}

void user_led_toggle(size_t i)
{
    switch(i){
        case 0: *GPIOD_ODR ^= (0x1u<<ODR1);break;
        case 1: *GPIOD_ODR ^= (0x1u<<ODR0);break;
        case 2: *GPIOC_ODR ^= (0x1u<<ODR10);break;
    }
}

size_t user_dipswitch_count(void) { return USER_NUM_DIP_SWITCHES; } 
uint32_t user_dipswitch_read(size_t idx){
    if(idx==0){
        uint32_t dip = ((*GPIOB_IDR & (1<<IDR4)) >> 4) |
                       ((*GPIOB_IDR & (1<<IDR3)) >> 2) |
                       ((*GPIOD_IDR & (1<<IDR7)) >> 5) |
                       ((*GPIOD_IDR & (1<<IDR6)) >> 3) |
                       ((*GPIOD_IDR & (1<<IDR5)) >> 1) |
                       ((*GPIOD_IDR & (1<<IDR4)) << 1) ;
        return ~dip & 0x3f;
    }
    if(idx==1){
        uint32_t dip =
                       ((*GPIOA_IDR & (1<<IDR5))     ) |
                       ((*GPIOA_IDR & (1<<IDR6)) >> 2) |
                       ((*GPIOA_IDR & (1<<IDR7)) >> 4) |
                       ((*GPIOC_IDR & (1<<IDR4)) >> 2) |
                       ((*GPIOC_IDR & (1<<IDR5)) >> 4) |
                       ((*GPIOB_IDR & (1<<IDR0))     ) ;
        return ~dip & 0x3f;
    }
    return -1;
}

size_t user_serial_count(void) { return USER_NUM_SERIALS; }

void user_serial_write_char(size_t idx, int c) {
    if(idx==0)
        usart1_write_char(c);
}

int user_serial_read_char(size_t idx) {
    if(idx==0)
        return usart1_read_char();
    return -1;
}

/* used by lib/printf */
void _putchar(char c)
{
    if(c=='\n')
        usart1_write_char('\r');
    usart1_write_char((int)c);
}

/* used by newlib (like, #include <stdio.h>) */
int _write(int fd, const void* buf, size_t len){
    (void)fd;
    const char* p = (const char*)buf;
    for(size_t i=0;i<len;i++) {
        if((int)p[i]=='\n')
            usart1_write_char('\r');
        usart1_write_char((int)p[i]);
    }
    return (int)len;
}

size_t user_enc_count(void) { return USER_NUM_ENCODERS; }

user_enc_sample_t user_enc_read(size_t idx)
{
    (void)idx;
    static int16_t last = 0x8000;
    int16_t now = (int16_t)*TIM4_CNT;          // encoder position (16-bit)
    int16_t d16 = (int16_t)(now - last);       // wrap-safe signed delta
    last = now;

    return (user_enc_sample_t){ .value = now, .delta = d16, .pushed = 0 };
}

/* assuming 1ms systick, it should be placed in the systick routine */
void user_systick_service(void) {
    oled_systick_dma();
}

void user_io_init(void) {
    user_btn_init();
    user_dipswitch_init();
    user_led_init();
    user_enc_init();
    user_serial_init();
}


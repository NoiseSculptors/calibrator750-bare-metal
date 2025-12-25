
// user_io.c (strong overrides)
#include "user_io.h"
#include "user_io_config.h"
#include "gpio.h"
#include "rcc.h"
#include "printf.h"
#include "serial.h"
#include "syscall.h"
#include "timer.h"

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

void user_io_init(void) {
    user_btn_init();
    user_dipswitch_init();
    user_led_init();
    user_enc_init();
    user_serial_init();
}

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

size_t user_display_count(void) { return USER_NUM_DISPLAYS; }
void user_display_flush(size_t i, const void* buf, size_t n)
{
    (void)i;
    (void)buf;
    (void)n;
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


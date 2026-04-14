
/*
   B10 LCD PWR
   B11 LED TIM2_CH4  AF1 
   B12 SPI2_NSS  AF5
   B13 SPI2_SCK  AF5
   B14 RS (DC, DATA/COMMAND)
   B15 SPI2_MOSI AF5
    D8 LCD RESET
 */

#include "delay.h"
#include "dma.h"
#include "dmamux.h"
#include "gpio.h"
#include "init.h"
#include "spi_lcd.h"
#include "nvic.h"
#include "printf.h"
#include "rcc.h"
#include "spi.h"
#include "timer.h"
#include <stdint.h>

// 320x240(16bit RGB 5-6-5)
extern uint16_t fb[WIDTH*HEIGHT] __attribute__((aligned(32))); 
static uint8_t cmd[4] __attribute__((aligned(32))); 
static uint8_t parameter[4] __attribute__((aligned(32))); 
static inline void LCD_DC_L(void)  { *GPIOB_ODR &= ~(1u<<14);  }
static inline void LCD_DC_H(void)  { *GPIOB_ODR |=  (1u<<14);  }

static inline void delay_cmd(void)
{
    delay_us(3);
}

static inline void delay_fb(void)
{
    delay_ms(7);
}

void led_pwm_init(uint32_t arr)
{
    *TIM2_PSC  = 0;                   // optional prescaler
    *TIM2_CR1 |= (1U << 7);           // ARPE = 1 (auto-reload preload)
    *TIM2_ARR  = arr;                 // set period

    /* CH4: OC4M = PWM mode 1 (110), OC4PE = 1 (preload) */
    *TIM2_CCMR2 &= ~(7U << 12);       // clear OC4M[2:0]
    *TIM2_CCMR2 |= (6U << 12) | (1U << 11);

    /* enable CH4 output (inverted set bit 13) */
    *TIM2_CCER |= (1U << 13)          // CC4P = 0
                | (1U << 12);         // CC4E = 1 (enable)

    *TIM2_EGR  = (1U << 0);           // UG: load shadow registers
    *TIM2_CR1 |= (1U << 0);           // CEN: start counter
}

void led_brightness(uint8_t pct)
{
    if (pct > 100) pct = 100;
    uint32_t arr = *TIM2_ARR;
    uint32_t val = ((uint32_t)(arr + 1) * pct) / 100;
    *TIM2_CCR4 = val;
    /* apply immediately if preload is enabled */
    *TIM2_EGR = (1U << 0);
}

void init_dma_spi(void){

    uint32_t dma_request_spi2_tx_dma = 40;

    // Clocks
    *RCC_AHB1ENR |= (1u<<0); // DMA1EN
        *GPIOB_ODR &= ~(1<<10);
        *GPIOB_ODR &= ~(1<<10);

    // DMAMUX: route spi2_tx_dma to DMA1 Stream0
    *DMAMUX1_C0CR = 0; // clear first
    *DMAMUX1_C0CR = (dma_request_spi2_tx_dma & 0x7F);

    // Make sure stream disabled
    *DMA1_S0CR &= ~1u;
    while (*DMA1_S0CR & 1u) {}

    // Addresses
    *DMA1_S0PAR  = (uint32_t)SPI2_TXDR; // peripheral = SPI->TXDR

    // CR: M2P | MINC | 8/8-bit | priority | (optional) TCIE/TEIE
    *DMA1_S0CR = (1u<<10) // MINC=1
        | (1u<<6); // DIR = Memory to peripheral
}

static void spi_dma_send(uint8_t *arr, uint16_t n)
{
    /* non-blocking - dma is triggered only if last transaction is finished
     * transfered */
    if (*DMA1_S0CR & 1u)
        return;

    // disable TXDMA
    *SPI2_CFG1  &= ~(1u<<15);  // TXDMAEN disable
    *SPI2_CR1 &= ~1;       // SPI disable

    *DMA1_LIFCR = (1u<<5)  // CTCIF0
        |(1u<<3); // CTEIF0

    *DMA1_S0M0AR = (uint32_t)arr;
    *DMA1_S0NDTR = n;

    /* trigger transfer */
    *SPI2_CFG1 |= (1<<15); // TXDMAEN enable
    *DMA1_S0CR |= (1<<0); // enable
    *SPI2_CR1 |= 1; // SPI enable
    *SPI2_CR1 |= (1<<9);  // CSTART
}

static void lcd_command(uint8_t n)
{
    LCD_DC_L();
    cmd[0] = n;
    spi_dma_send(cmd, 1);
}

static void lcd_data(uint8_t *arr, uint16_t n)
{
    LCD_DC_H();
    spi_dma_send(arr, n);
}

static void print_status(uint32_t status_reg)
{
    static uint32_t old_status;
    if(status_reg!=old_status){
        for(int i=0;i<=28;i++){
            uint8_t bit = (status_reg >> i) & 1;
            if(bit){
                if(i==0) printf("RXP ");
                if(i==1) printf("TXP ");
                if(i==2) printf("DXP ");
                if(i==3) printf("EOT ");
                if(i==4) printf("TXF ");
                if(i==5) printf("UDR ");
                if(i==6) printf("OVR ");
                if(i==7) printf("CRCE ");
                if(i==8) printf("TIFRE ");
                if(i==9) printf("MODF ");
                if(i==10) printf("TSERF ");
                if(i==11) printf("SUSP ");
                if(i==12) printf("TXC ");
                if(i==13||i==14) printf("RXPLVL: %d", ((status_reg>>13) & 3));
                if(i==15) printf("RXWNE ");
            }
        }
        printf("\n");
    }
    old_status = status_reg;
}

void init_pins(void){
    *RCC_APB1LENR &= ~((1u<<0)|(1u<<14)); // TIM2EN | SPI2EN
    *RCC_APB1LENR |= (1u<<0)|(1u<<14); // TIM2EN | SPI2EN
    *RCC_APB1LRSTR |= (1u<<14);  // SPI2RST
    *RCC_APB1LRSTR &= ~(1u<<14); // SPI2RST
    *RCC_AHB4ENR  |= (1u<<GPIOBEN)|(1u<<GPIODEN);
    gpio_ctrl(GPIOB, GPIO_MODE, GPIO11|GPIO12|GPIO13|GPIO15, MODE_AF);
    gpio_ctrl(GPIOB, GPIO_MODE, GPIO10|GPIO14, MODE_OUTPUT);
    gpio_ctrl(GPIOB, GPIO_AFRH, GPIO12|GPIO13|GPIO15, AF5); // SPI2
    gpio_ctrl(GPIOB, GPIO_AFRH, GPIO11, AF1); // TIM2
    gpio_ctrl(GPIOD, GPIO_MODE, GPIO8, MODE_OUTPUT);
    gpio_ctrl(GPIOB, GPIO_OSPEED, GPIO12|GPIO13|GPIO15, 1);
}

void spi_init(void)
{
    /* RCC_D2CCIP1R defaults SPI2, SPI123SEL[2:0] to
       pll1_q_ck (48MHz after boot).
       ST7789T3, max clock is determined by (according to datasheet) min 7ns
       clock high signal and min 7ns low, 14ns = 71.4MHz, ideally PLL1 Q set to
       142.8MHz
     */

    /* pll2q to 143MHz, 71.5MHz SPI clock*/
    // pll_2_start(PLLSRC_HSE, 2, 11, 0u, 1, 1, 128, 26000000u);

    /* pll2q to ~160.3MHz, for ~80.15MHz SPI clock, max I could achieve with
     * jumper wires */
    pll_2_start(PLLSRC_HSE, 2, 37, 0u, 3, 1, 128, 26000000u);

    *RCC_D2CCIP1R &= ~(7u<<12);
    *RCC_D2CCIP1R |= (1u<<12); // PLL2Q as clock source for SPI123

     *SPI2_CFG1 =
        (0 << 28) |     // 30:28 baud rate, 0:max speed, 7:slowest
        (8-1);          // bits per frame (n-1)
    *SPI2_CFG2 =
        (1u << 30) |    // SSOM output management in master mode
        (1u << 29) |    // SSOE  hardware NSS output
        (1u << 22) |    // MASTER
        (1u << 17) ;    // COMM  half-duplex
    *SPI2_CR1 =
        (1<<11);        // HDDIR half-duplex, transmitter only
}

static void lcd_pwr(uint8_t state)
{
    if(state)
        *GPIOB_ODR &= ~(1<<10);
    else
        *GPIOB_ODR |= (1<<10);
}

void lcd_init(void)
{
    lcd_pwr(0); delay_ms(120); lcd_pwr(1);
    lcd_command(0x01); delay_ms(150);    // SWRESET
    lcd_command(0x11); delay_ms(120);    // SLEEPOUT
    lcd_command(0x3A); delay_cmd();      // COLMOD
    parameter[0]=0x55;
    lcd_data(parameter,1); delay_cmd();  // COLMOD = 65K 16bit/pixel
    lcd_command(0x36); delay_cmd();     // MADCTL
    uint8_t madctl=0b01100000;                
    parameter[0] = madctl;
    lcd_data(parameter, 1); delay_cmd();
    lcd_command(0x21); delay_cmd();     // INVON

    uint16_t width = 320-1;
    uint16_t height = 240-1;
    // caset
    parameter[0]=0;
    parameter[1]=0;
    parameter[2]=(uint8_t)(width>>8);
    parameter[3]=(uint8_t)(width & 0xff);
    lcd_command(0x2A); delay_cmd();
    lcd_data(parameter, 4); delay_cmd();

    // raset
    parameter[0]=0;
    parameter[1]=0;
    parameter[2]=(uint8_t)(height>>8);
    parameter[3]=(uint8_t)(height & 0xff);
    lcd_command(0x2B); delay_cmd();
    lcd_data(parameter, 4); delay_cmd();

    lcd_command(0x29); delay_ms(20);    // DISPLAY ON
}

void IRQ_SPI2_Handler(void)
{
    /* if needed */
}

void lcd_flush_fb(uint16_t *framebuffer)
{
    // ramwr
    lcd_command(0x2c); delay_cmd();

    uint8_t *p = (uint8_t *)framebuffer;

    lcd_data(p, 65535); delay_fb();
    p+=65535;
    lcd_data(p, 65535); delay_fb();
    p+=65535;
    lcd_data(p, 22530); delay_ms(1);
}

void fb_put_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if ((uint32_t)x >= WIDTH || (uint32_t)y >= HEIGHT) return;
    uint32_t idx = ((uint32_t)y * WIDTH + x) << 1;
    uint16_t px = ((uint16_t)(r >> 3) << 11) | ((uint16_t)(g >> 2) << 5) | (b >> 3);
    fb[idx]     = (uint8_t)(px >> 8);
    fb[idx + 1] = (uint8_t)px;
}


void lcd_hard_reset(void)
{
    *GPIOD_ODR &= ~(1<<8);
    delay_ms(10);
    *GPIOD_ODR |= (1<<8);
    delay_ms(120);
}

void fpu_enable(void){
    // CP10/CP11 full access
    *(volatile uint32_t*)0xE000ED88 |= (0xFu << 20);
    __asm volatile("dsb; isb");
}


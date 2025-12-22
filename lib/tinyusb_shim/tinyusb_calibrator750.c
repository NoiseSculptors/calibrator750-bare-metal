
#include "delay.h"
#include "gpio.h"
#include "init.h"
#include "nvic.h"
#include "nvic_exti.h"
#include "pwr.h"
#include "rcc.h"
#include "systick.h"
#include "usb.h"
#include "user_io.h"
#include <stdio.h>

/* tinyusb */
#include "tusb.h"
#include "bsp/board_api.h"

/* for implementation details, follow tinyusb/hw/bsp/stm32h7/family.c */

uint32_t milliseconds;
extern clock_info_t ci;

void board_init(void){

  init_clock();
  user_io_init(); /* define your own setup in userio/user_io.c */
  init_systick_1ms();

  milliseconds = 0;
  SystemCoreClock = ci.hclk_hz;

  irq_enable(IRQ_OTG_FS);
  // irq_enable(IRQ_OTG_HS);

  // ------ GPIO clocks: enable GPIOA (USB DM/DP) -----------------------------
  *RCC_AHB4ENR |= (1u << GPIOAEN);

  // ------ Configure PA11/PA12 as USB FS DM/DP (AF10, medium speed) ----------
  *GPIOA_MODER &= ~((3u << (11*2)) | (3u << (12*2)));
  *GPIOA_MODER |=  ((2u << (11*2)) | (2u << (12*2))); // set AF mode

  // OTYPER: push-pull (0)
  *GPIOA_OTYPER &= ~((1u << 11) | (1u << 12));

  // OSPEEDR: medium speed
  *GPIOA_OSPEEDR &= ~((3u << (11*2)) | (3u << (12*2)));
  *GPIOA_OSPEEDR |=  ((1u << (11*2)) | (1u << (12*2)));

  // PUPDR: 00b = No pull
  *GPIOA_PUPDR &= ~((3u << (11*2)) | (3u << (12*2)));

  // AF10 = OTG2_FS
  const uint32_t OTG2_FS = 10u;
  *GPIOA_AFRH &= ~((0xFu << AFR11) | (0xFu << AFR12));
  *GPIOA_AFRH |=  (OTG2_FS << AFR11) | (OTG2_FS << AFR12);

  *RCC_D2CCIP2R &= ~(1u << 20); // USBSEL *0:none*, 1:PLL1Q, 2:PLL3Q, 3:HSI48


  // ------ Enable USB2 OTG FS peripheral clock -------------------------------
  /* pll1_q_clk@48MHz used as clock source */
  *RCC_D2CCIP2R |= (1u << 20); // USBSEL *1:PLL1Q*, 2:PLL3Q, 3:HSI48
  *RCC_AHB1ENR |= (1u << 27); // 27:USB2OTGHSEN|28:USB2OTGHSULPIEN (not needed)
  // not needed:
  // *USB2_OTG_FS_OTG_GRSTCTL |= (1<<1); /* CSRST: Core soft reset */
  *USB2_OTG_FS_OTG_GCCFG |= (1u << 16); // PWRDWN (1:USB FS PHY enabled)
  // *USB2_OTG_FS_OTG_GCCFG &= ~(1u << 21); // VBDEN turn off VBUS detection
                                            // (default after reset)

  *PWR_CR3 |= (1<<24); // USB33DEN  USB33 voltage level detector enabled.
  while(!(*PWR_CR3 & (1<<26))){} // USB33RDY wait until USB33 detector ready

#if 0 /* TODO: host mode */
#if CFG_TUH_ENABLED == 1
  printf("---------------------------------------------\n"
         "tinyusb_calibrator750.c / selecting host mode\n");
  *USB2_OTG_FS_OTG_HCFG    |= (1<<0); /* FSLSPCS[1:0]: FS/LS PHY clock select
When the core is in FS host mode */
  *USB2_OTG_FS_OTG_HPRT    |= (1<<6); /* port reset */
  delay_ms(10);
  *USB2_OTG_FS_OTG_HPRT    &= ~(1<<6); /* release port reset */
  *USB2_OTG_FS_OTG_HCCHAR0 |= (64<<0); /*  Maximum packet size */
  *USB2_OTG_FS_OTG_GUSBCFG |= (1<<29); /* force host mode */

#endif
#endif

}

void SysTick_Handler(void){
    milliseconds++;
    if(milliseconds%500==0)
        user_led_toggle(1);
}

void IRQ_OTG_FS_Handler(void){
    tusb_int_handler(0,1);
}

void board_init_after_tusb(void){
    /* nothing needed */
}

uint32_t tusb_time_millis_api(void){
    return milliseconds;
}

uint32_t board_millis(void){
    return milliseconds;
}

void board_led_write(int led_status){
    user_led_write(0,led_status);
}

#define UID_W0   (volatile unsigned int *)(0x1FF1E800)
#define UID_W1   (volatile unsigned int *)(0x1FF1E804)
#define UID_W2   (volatile unsigned int *)(0x1FF1E808)
size_t board_get_unique_id(uint8_t id[], size_t max_len){
    (void)max_len;
    const uint8_t len = 12;
    uint8_t raw[len];
    // Endianness: UID_Wx are 32-bit words; pack LSB-first into raw[]
    uint32_t w0 = *UID_W0, w1 = *UID_W1, w2 = *UID_W2;
    raw[0] = (uint8_t)(w0 >> 0);
    raw[1] = (uint8_t)(w0 >> 8);
    raw[2] = (uint8_t)(w0 >> 16);
    raw[3] = (uint8_t)(w0 >> 24);
    raw[4] = (uint8_t)(w1 >> 0);
    raw[5] = (uint8_t)(w1 >> 8);
    raw[6] = (uint8_t)(w1 >> 16);
    raw[7] = (uint8_t)(w1 >> 24);
    raw[8] = (uint8_t)(w2 >> 0);
    raw[9] = (uint8_t)(w2 >> 8);
    raw[10]= (uint8_t)(w2 >> 16);
    raw[11]= (uint8_t)(w2 >> 24);
 
    for (size_t i = 0; i < len; ++i) id[i] = raw[i];
    return len;
}

static inline uint8_t hex_nibble(uint8_t v) {
    v &= 0x0Fu;
    return (uint8_t)("0123456789ABCDEF"[v]);
}

size_t board_usb_get_serial(uint16_t desc_str1[], size_t max_chars){
    // Each byte -> two hex chars; each hex char -> one UTF-16 code unit
    uint8_t tmp[12];
    size_t uid_len = board_get_unique_id(tmp, sizeof(tmp));

    if (max_chars == 0) return 0;
    size_t out = 0;

    for (size_t i = 0; i < uid_len && out + 2 <= max_chars; ++i) {
        uint8_t b = tmp[i];
        uint8_t hi = hex_nibble((uint8_t)(b >> 4));
        uint8_t lo = hex_nibble((uint8_t)(b & 0x0F));
        desc_str1[out++] = (uint16_t)hi;
        if (out < max_chars) desc_str1[out++] = (uint16_t)lo;
    }
    return out; // number of UTF-16 code units written
}

int board_getchar(void)
{
    return 0;
}

uint32_t board_button_read(void)
{
    return (uint32_t)user_btn_read(0);
}

void board_reset_to_bootloader(void)
{
    /* not implemented */
}

void board_delay(uint32_t ms)
{
    while(ms){
#if CFG_TUD_ENABLED
        tud_task();
#endif
#if CFG_TUH_ENABLED
        tuh_task();
#endif
        delay_ms(1);
        ms--;
    }
}


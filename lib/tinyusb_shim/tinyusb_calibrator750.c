
#include "gpio.h"
#include "init.h"
#include "nvic.h"
#include "nvic_exti.h"
#include "pwr.h"
#include "rcc.h"
#include "systick.h"
#include "usb.h"
#include "user_io.h"

/* tinyusb */
#include "tusb.h"
#include "bsp/board_api.h"

/* for implementation details, follow tinyusb/hw/bsp/stm32h7/family.c */

uint32_t milliseconds;
extern clock_info_t ci;

void board_init(void){


  milliseconds = 0;
  SystemCoreClock = ci.hclk_hz;

  user_led_init();

  irq_enable(IRQ_OTG_FS);
  irq_enable(IRQ_OTG_HS);

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

  // ------ Enable USB2 OTG FS peripheral clock -------------------------------
  /* pll1_q_clk@48MHz used as clock source */
  *RCC_D2CCIP2R |= (1u << 20); // USBSEL *1:PLL1Q*, 2:PLL3Q, 3:HSI48
  *RCC_AHB1ENR |= (1u << 27); // 27:USB2OTGHSEN|28:USB2OTGHSULPIEN (not needed)
  *USB2_OTG_FS_OTG_GCCFG |= (1u << 16); // PWRDWN (1:USB FS PHY enabled)
  // *USB2_OTG_FS_OTG_GCCFG &= ~(1u << 21); // VBDEN turn off VBUS detection
                                            // (default after reset)

  // Included in tinyusb code, but it seems not needed
  // 7:BVALOVAL: B-peripheral session valid override value.
  // 6:BVALOEN:  B-peripheral session valid override enable.
  //*USB2_OTG_FS_OTG_GOTGCTL |= (1<<7)|(1<<6);

  *PWR_CR3 |= (1<<24); // USB33DEN  USB33 voltage level detector enabled.
  while(!(*PWR_CR3 & (1<<26))){} // USB33RDY wait until USB33 detector ready

  // ------ SysTick @ 1 kHz ---------------------------------------------------
  uint32_t systick_clk = ci.hclk_hz / 8u;
  uint32_t reload = (systick_clk / 1000u) - 1u; // 1kHz tick

  *SYST_RVR = reload & 0x00FFFFFFu; // clamp to 24-bit
  *SYST_CVR = 0; // clear current
  *SYST_CSR = (1<<SYST_CSR_CLKSOURCE) |
              (1<<SYST_CSR_TICKINT) |
              (1<<SYST_CSR_ENABLE);
}

void SysTick_Handler(void){
    milliseconds++;
    user_led_toggle(1); // 1kHz
}

void IRQ_OTG_FS_Handler(void){
    tusb_int_handler(0,1);
}

void board_init_after_tusb(void){
    /* nothing needed */
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


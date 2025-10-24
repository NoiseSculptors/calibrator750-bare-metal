#include "gpio.h"
#include "init.h"
#include "printf.h"
#include "rcc.h"
#include "sdmmc.h"
#include "serial.h"
#include <stdint.h>

// ---------- tiny utils ----------
static inline void delay_loop(volatile uint32_t n){ while(n--) __asm__("nop"); }
static int wait_flag_set(volatile uint32_t *reg, uint32_t mask, uint32_t to){
    while(to--){ if((*reg)&mask) return 0; } return -1;
}
static int wait_flag_clear(volatile uint32_t *reg, uint32_t mask, uint32_t to){
    while(to--){ if(((*reg)&mask)==0) return 0; } return -1;
}

// ---------- GPIO + RCC ----------
static void init_sdmmc(void)
{
    // Enable GPIOC/D
    *RCC_AHB4ENR |= (0x1u<<GPIODEN) | (0x1u<<GPIOCEN);

    // PC12 = CK (AF12), very high speed, no pull
    *GPIOC_MODER   &= ~(3u<<(12*2));      // alt
    *GPIOC_MODER   |=  (2u<<(12*2));
    *GPIOC_OSPEEDR |=  (1u<<(12*2));     // very high
    *GPIOC_PUPDR   &= ~(3u<<(12*2));      // no pull
    *GPIOC_AFRH    &= ~(0xFu<<AFR12);
    *GPIOC_AFRH    |=  (12u<<AFR12); // AF12

    // PD2 = CMD (AF12), very high speed, pull-up
    *GPIOD_MODER   &= ~(3u<<(2*2));       // alt
    *GPIOD_MODER   |=  (2u<<(2*2));
    *GPIOD_OSPEEDR |=  (1u<<(2*2));
    *GPIOD_PUPDR   &= ~(3u<<(2*2));
    *GPIOD_PUPDR   |=  (1u<<(2*2));       // PU
    *GPIOD_AFRL    &= ~(0xFu<<AFR2);
    *GPIOD_AFRL    |=  (12u<<AFR2);      // AF12

    // PC8 = D0 (AF12), very high speed, pull-up
    *GPIOC_MODER   &= ~(3u<<(8*2));       // alt
    *GPIOC_MODER   |=  (2u<<(8*2));
    *GPIOC_OSPEEDR |=  (1u<<(8*2));
    *GPIOC_PUPDR   &= ~(3u<<(8*2));
    *GPIOC_PUPDR   |=  (1u<<(8*2));       // PU
    *GPIOC_AFRH    &= ~(0xFu<<AFR8);
    *GPIOC_AFRH    |=  (12u<<AFR8);  // AF12


    // SDMMC1 kernel clock source: bit 16 (1=PLL2R, 0=PLL1Q)
    *RCC_D1CCIPR &= ~(1u<<16);                 // use PLL1Q (adjust if you prefer PLL2R)
    // Enable SDMMC1 (and delay) clock on AHB3
    *RCC_AHB3RSTR |= (1u<<16);
    *RCC_AHB3RSTR &= ~(1u<<16);
    *RCC_AHB3ENR |= (1u<<16);                  // SDMMC1EN
}

static uint32_t sd_calc_clkdiv_h7(uint32_t kernel_hz, uint32_t target_hz)
{
    if (!target_hz) return 0x3FF;
    // DIV = (f_kernel / (2*target)) - 2
    uint32_t div = kernel_hz / (2u * target_hz);
    if (div > 0) div -= 2u;
    if ((int32_t)div < 0) div = 0;
    if (div > 0x3FFu) div = 0x3FFu;
    return div;
}

static void sdmmc_power_on(void)
{
    // PWRCTRL = 0b11 : power on + 74 cycles
    *SDMMC1_POWER = 3u;
    delay_loop(10000);
}

static void sdmmc_set_clock(uint32_t kernel_hz, uint32_t target_hz)
{
    uint32_t div = sd_calc_clkdiv_h7(kernel_hz, target_hz);

#if 0
    *SDMMC1_CLKCR &= ~((0x3FFu<<0)   /* CLKDIV[9:0] */
             | (3u<<14)      /* WIDBUS      */
             | (1u<<17)      /* NEGEDGE     */
             | (1u<<18)      /* HWFC_EN     */
             | (1u<<19)      /* DDR         */
             | (1u<<20)      /* BUSSPEED    */
             | (3u<<21));    /* SELCLKRX    */
#endif

    *SDMMC1_CLKCR = 0;
    *SDMMC1_CLKCR |= (div<<0); /* 1-bit bus, posedge, normal speed, receive on
                                  sdmmc_ck */
}

// ---------- CMD helpers ----------
enum sd_resp { SD_NO_RESP, SD_R1, SD_R1b, SD_R2, SD_R3, SD_R6, SD_R7 };

static int sd_send_cmd(uint32_t idx, uint32_t arg, enum sd_resp rtype, uint32_t *resp)
{
    *SDMMC1_ICR  = 0xFFFFFFFFu;   // clear static flags
    *SDMMC1_ARGR = arg;

    uint32_t cmd = 0;
    cmd |= (idx & 0x3Fu);         // CMDINDEX
    cmd |= (1u<<10);              // CPSMEN (start)

    // WAITRESP (bits 7:6) + LONGRESP (bit 7 meaning combined with 6) per your header names
    switch (rtype) {
        case SD_NO_RESP: break;
        case SD_R2:  cmd |= (1u<<7) | (1u<<6); break; // long resp
        case SD_R3:  cmd |= (1u<<6); break;           // short
        case SD_R7:  cmd |= (1u<<6); break;
        case SD_R6:  cmd |= (1u<<6); break;
        case SD_R1:  cmd |= (1u<<6); break;
        case SD_R1b: cmd |= (1u<<6) | (1u<<8); break; // wait for interrupt/busy
    }

    *SDMMC1_CMDR = cmd;

    // Wait for CMDSENT (no resp) or CMDREND (resp), or error
    uint32_t to = 1000000;
    while (to--) {
        uint32_t sta = *SDMMC1_STAR;
        if (rtype == SD_NO_RESP) {
            if (sta & (1u<<7)) {
                *SDMMC1_ICR = (1u<<7); break; }  // CMDSENT
        } else {
            if (sta & (1u<<6)) { *SDMMC1_ICR = (1u<<6); break; }  // CMDREND
        }
        if (sta & ((1u<<0)/*CCRCFAIL*/ | (1u<<2)/*CTIMEOUT*/)) {
            *SDMMC1_ICR = (1u<<0) | (1u<<2);
            return -1;
        }
    }
    if ((int32_t)to <= 0) return -2;

    if (rtype == SD_R2) {
        if (resp) {
            resp[0] = *SDMMC1_RESP1R;
            resp[1] = *SDMMC1_RESP2R;
            resp[2] = *SDMMC1_RESP3R;
            resp[3] = *SDMMC1_RESP4R;
        }
    } else if (rtype != SD_NO_RESP) {
        if (resp) resp[0] = *SDMMC1_RESP1R;
    }
    return 0;
}

static int sd_send_acmd(uint16_t rca, uint32_t acmd_idx, uint32_t arg, enum sd_resp rtype, uint32_t *resp)
{
    int rc = sd_send_cmd(55, ((uint32_t)rca)<<16, SD_R1, resp);
    if (rc) return rc;
    return sd_send_cmd(acmd_idx, arg, rtype, resp);
}

// ---------- Data read (PIO, 512B) ----------
static int sd_read_block(uint32_t lba, uint8_t *buf512)
{
    // Timer/DLEN/DCTRL
    *SDMMC1_DTIMER = 0x00FFFFFFu;
    *SDMMC1_DLENR  = 512;
    *SDMMC1_DCTRLR = 0;
    *SDMMC1_DCTRLR |= (9u<<4);   // block size = 2^9 = 512
    *SDMMC1_DCTRLR |= (1u<<1);   // DTDIR = from card to host

    *SDMMC1_ICR = 0xFFFFFFFFu;

    // SDHC/SDXC: LBA addressing
    int rc = sd_send_cmd(17, lba, SD_R1, NULL); // READ_SINGLE_BLOCK
    printf("rc:...%d\n",rc);
    if (rc) return rc;

    *SDMMC1_DCTRLR |= (1u<<0);  // DTEN

    uint32_t *p32 = (uint32_t*)buf512;
    uint32_t words = 512/4;

    while (words) {
        uint32_t sta = *SDMMC1_STAR;
        printf("%08x\n",*SDMMC1_STAR);

        if (sta & (1u<<21)) { // RXFIFOHF (>= 8 words)
            for (int i=0; i<8 && words; i++, words--) {
                *p32++ = *SDMMC1_FIFOR;
            }
            continue;
        }
        if (sta & (1u<<5)) {  // RXDAVL (>= 1 word)
            *p32++ = *SDMMC1_FIFOR;
            words--;
            continue;
        }
        if (sta & ((1u<<3)/*DCRCFAIL*/ | (1u<<4)/*DTIMEOUT*/ | (1u<<1)/*RXOVERR*/)) {
            *SDMMC1_ICR = (1u<<3)|(1u<<4)|(1u<<1);
            return -10;
        }
        if (sta & (1u<<8)) {  // DATAEND
            if (words == 0) { *SDMMC1_ICR = (1u<<8); break; }
        }
    }
    return 0;
}

// ---------- Card init ----------
static int sd_card_init(uint32_t kernel_hz)
{
    sdmmc_power_on();
    delay_loop(100000);

    // Identify clock ≈ 400 kHz
    sdmmc_set_clock(kernel_hz, 800000u);

    *SDMMC1_DCTRLR = 0;             // ensure DPSM idle
    *SDMMC1_ICR    = 0xFFFFFFFFu;   // clear flags

    // CMD0
    if (sd_send_cmd(0, 0, SD_NO_RESP, NULL)) return -1;

    // CMD8 (if it times out, continue)
    uint32_t r[4] = {0};
    (void)sd_send_cmd(8, 0x000001AAu, SD_R7, r);

    // ACMD41 loop (HCS=1, 3.2–3.3V)
    uint32_t ocr = 0;
    for (int i=0; i<2000; i++) {
        if (sd_send_acmd(0, 41, 0x40300000u, SD_R3, r)) return -2;
        ocr = r[0];
        if (ocr & (1u<<31)) break;     // busy clear
        delay_loop(50000);
    }
    if (!(ocr & (1u<<31))) return -3;

    // CMD2 (CID), CMD3 (RCA)
    if (sd_send_cmd(2, 0, SD_R2, r)) return -4;
    if (sd_send_cmd(3, 0, SD_R6, r)) return -5;
    uint16_t rca = (uint16_t)(r[0] >> 16);

    // CMD7 (select) — simple wait is fine here
    if (sd_send_cmd(7, ((uint32_t)rca)<<16, SD_R1b, NULL)) return -6;

    // CMD16 to 512 (ignored by SDHC but harmless)
    (void)sd_send_cmd(16, 512, SD_R1, NULL);

    // Raise to a gentle transfer clock (e.g. 2 MHz first)
    sdmmc_set_clock(kernel_hz, 2000000u);

    return 0;
}

// ---------- Demo ----------
static uint8_t g_block[512];

int sdmmc_baremetal_read_test(void)
{
    // Set this to your real SDMMC kernel frequency (PLL1Q or PLL2R).
    const uint32_t SDMMC_KERNEL_HZ = 48000000u;

    int rc = sd_card_init(SDMMC_KERNEL_HZ);
    printf("init rc=%d\n", rc);
 //   if (rc) return rc;

    rc = sd_read_block(0, g_block);
    printf("read rc=%d\n", rc);
  //  if (!rc) {
        for (int i=0; i<512; i++) {
            if ((i & 0x0F) == 0) printf("\n%03x: ", i);
            printf("%02x ", g_block[i]);
        }
        printf("\n");
  //  }
    return rc;
}

int main(void)
{

    clock_info_t ci;
    ci = init_clock();
    init_serial(&ci, 115200);

    init_sdmmc();

    printf("\033[2J\033[H");
    printf("SDMMC test program\n");
    int rc = sdmmc_baremetal_read_test();
    printf("CLKCR=0x%08lx\n", *SDMMC1_CLKCR);
    printf("POWER=0x%08lx\n", *SDMMC1_POWER);
    printf("D1CCIPR=0x%08lx\n", *RCC_D1CCIPR);
    printf("done rc=%d\n", rc);
    while(1){}
}


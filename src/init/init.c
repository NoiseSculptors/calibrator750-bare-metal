
#include "rcc.h"
#include "pwr.h"
#include "flash.h"
#include "syscfg.h"
#include "init.h"
#include "vtor.h"
#include <stdint.h>

/* ---- User PLL plan ---- */
#define HSE 26000000u

clock_info_t init_clock(void) {
    return init_clock_adv(13, 240, 1, 10, 2);
}

clock_info_t init_clock_adv (
        uint32_t M,
        uint32_t N,
        uint32_t P,
        uint32_t Q,
        uint32_t R
        )
{
    clock_info_t ci = {0};

    /*  Power / voltage scaling to VOS0 (overdrive)  */
    #define SCUEN       2
    #define LDOEN       1
    *PWR_CR3 |= (1u << SCUEN) | (1u << LDOEN);

    #define ACTVOSRDY   13
    while (!(*PWR_CSR1 & (1u << ACTVOSRDY))) {}

    /*  HSE on (no bypass)  */
    #define HSEBYP      18
    #define HSERDY      17
    #define HSEON       16
    *RCC_CR &= ~(1u << HSEBYP);
    *RCC_CR |=  (1u << HSEON);
    while (!(*RCC_CR & (1u << HSERDY))) {}

    /*  VOS0 (boost)  */
    #define VOS         14      /* PWR_D3CR[15:14] */
    #define VOSRDY      13
    *PWR_D3CR = (*PWR_D3CR & ~(0x3u << VOS)) | (0x3u << VOS);

    #define SYSCFGEN    1
    #define ODEN        0
    *RCC_APB4ENR  |= (1u << SYSCFGEN);
    *SYSCFG_PWRCR |= (1u << ODEN);
    while (!(*PWR_D3CR & (1u << VOSRDY))) {}

    /*  Prescalers: D1CPRE=/1, HPRE=/2, APB*=/2  */
    #define D1CPRE      8
    #define HPRE        0
    #define D1PPRE      4
    *RCC_D1CFGR =
        (0x0u << D1CPRE) |    /* /1  (CPU domain) */
        (0x8u << HPRE)   |    /* /2  (AHB/HCLK)   */
        (0x4u << D1PPRE);     /* /2  (APB3/PCLK3) */

    #define D2PPRE1     4
    #define D2PPRE2     8
    *RCC_D2CFGR =
        (0x4u << D2PPRE1) |   /* /2 (APB1/PCLK1) */
        (0x4u << D2PPRE2);    /* /2 (APB2/PCLK2) */

    #define D3PPRE      4
    *RCC_D3CFGR = (0x4u << D3PPRE); /* /2 (APB4/PCLK4) */

    /*  PLL1: src=HSE, integer mode (no FRAC)  */
    #define PLLSRC      0       /* RCC_PLLCKSELR[1:0] */
    #define DIVM1       4       /* RCC_PLLCKSELR[9:4] (write actual value) */
    *RCC_PLLCKSELR =
        (0x2u << PLLSRC) |   /* 0b10 = HSE */
        (M  << DIVM1);       /* M */

    #define PLL1VCOSEL  1
    #define PLL1RGE     2       /* [3:2] */
    #define PLL1FRACEN  0
    #define DIVP1EN     16
    #define DIVQ1EN     17
    #define DIVR1EN     18
    *RCC_PLLCFGR =
        (0x1u << PLL1RGE) |  /* input range 2..4 MHz (RGE = 01) */
        (1u   << DIVP1EN) |
        (1u   << DIVQ1EN) |
        (1u   << DIVR1EN);
    *RCC_PLLCFGR &= ~(1u << PLL1VCOSEL);  /* wide VCO */
    *RCC_PLLCFGR &= ~(1u << PLL1FRACEN);  /* fractional mode OFF */

    /* value-1 encoding for N/P/Q/R in RCC_PLL1DIVR */
    #define DIVN1       0
    #define DIVP1       9
    #define DIVQ1       16
    #define DIVR1       24
    *RCC_PLL1DIVR =
        ((N - 1u) << DIVN1) |
        ((P - 1u) << DIVP1) |
        ((Q - 1u) << DIVQ1) |
        ((R - 1u) << DIVR1);

    /*  Enable PLL1 and wait ready  */
    #define PLL1ON      24
    #define PLL1RDY     25
    *RCC_CR |= (1u << PLL1ON);
    while (!(*RCC_CR & (1u << PLL1RDY))) {}

    /*  Flash latency + write high-frequency  */
    #define LATENCY     0
    #define WRHIGHFREQ  4
    *FLASH_ACR = ((*FLASH_ACR & ~((0x7u << LATENCY) | (0x3u << WRHIGHFREQ))) |
                  (0x7u << LATENCY) | (0x2u << WRHIGHFREQ));

    /*  Switch SYSCLK to PLL1 (P output)  */
    #define SW          0
    #define SWS         3
    *RCC_CFGR = (*RCC_CFGR & ~(0x3u << SW)) | (0x3u << SW); /* 0b11 = PLL1 */
    while (((*RCC_CFGR >> SWS) & 0x3u) != 0x3u) {}

    /* ========= Integer clock math (no fractional considered) ========= */

    /* fvco = (HSE / M) * N  (integer math) */
    uint32_t fref       = (uint32_t)(HSE / M);              /* assume divisible */
    ci.hse_hz           = HSE;
    ci.pll1_vco_hz      = (uint32_t)((uint64_t)fref * (uint64_t)N);

    ci.pll1_p_hz        = (P ? (ci.pll1_vco_hz / P) : 0u);
    ci.pll1_q_hz        = (Q ? (ci.pll1_vco_hz / Q) : 0u);
    ci.pll1_r_hz        = (R ? (ci.pll1_vco_hz / R) : 0u);

    ci.sysclk_hz        = ci.pll1_p_hz;

    /* HPRE=/2 → HCLK */
    ci.hclk_hz          = ci.sysclk_hz / 2u;

    /* APB prescalers all /2 here */
    ci.pclk1_apb1_hz         = ci.hclk_hz / 2u;  /* APB1 */
    ci.pclk2_apb2_hz         = ci.hclk_hz / 2u;  /* APB2 */
    ci.pclk3_apb3_hz         = ci.hclk_hz / 2u;  /* APB3 */
    ci.pclk4_apb4_hz         = ci.hclk_hz / 2u;  /* APB4 */

    /* Timer kernel clocks: if APB prescaler != /1 → timer_clk = PCLKx * 2 */
    ci.tim_apb1_hz      = ci.pclk1_apb1_hz * 2u; /* TIM2/3/4/5... */
    ci.tim_apb2_hz      = ci.pclk2_apb2_hz * 2u; /* TIM1/TIM8 */

    return ci;
}


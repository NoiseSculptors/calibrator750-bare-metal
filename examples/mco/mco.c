
#include "gpio.h"
#include "init.h"
#include "printf.h"
#include "rcc.h"
#include "user_io.h"
#include <stdint.h>

extern clock_info_t ci;

static void init_mco(void)
{
/*  From RM0433 Rev 8

    Bits 31:29 MCO2[2:0]: Micro-controller clock output 2

        Set and cleared by software. Clock source selection may generate
        glitches on MCO2.  It is highly recommended to configure these bits
        only after reset, before enabling the external oscillators and the
        PLLs.

        000: System clock selected (sys_ck) (default after reset)
        001: PLL2 oscillator clock selected (pll2_p_ck)
        010: HSE clock selected (hse_ck)
        011: PLL1 clock selected (pll1_p_ck)
        100: CSI clock selected (csi_ck)
        101:LSI clock selected (lsi_ck)


    Bits 28:25 MCO2PRE[3:0]: MCO2 prescaler

        Set and cleared by software to configure the prescaler of the MCO2.
        Modification of this prescaler may generate glitches on MCO2. It is
        highly recommended to change this prescaler only after reset, before
        enabling the external oscillators and the PLLs.

        0000: prescaler disabled (default after reset)
        0001: division by 1 (bypass)
        0010: division by 2
        0011: division by 3
        0100: division by 4
    ...
    1111: division by 15

    Bits 24:22 MCO1[2:0]: Micro-controller clock output 1

        Set and cleared by software. Clock source selection may generate
        glitches on MCO1.  It is highly recommended to configure these bits
        only after reset, before enabling the external oscillators and the
        PLLs.

        000: HSI clock selected (hsi_ck) (default after reset)
        001: LSE oscillator clock selected (lse_ck)
        010: HSE clock selected (hse_ck)
        011: PLL1 clock selected (pll1_q_ck)
        100: HSI48 clock selected (hsi48_ck)

    Bits 21:18 MCO1PRE[3:0]: MCO1 prescaler, same settings as MC02

*/
    *RCC_CFGR = 0; /* reset value */
    *RCC_CFGR |= (0<<29)   | /* MCO2 source */
                 (10u<<25) | /* MCO2 prescaler */
                 (2u<<22)  | /* MCO1 source */
                 (10u<<18) ; /* MCO1 prescaler */

/* 
    MCO1: PC9 AF0
    MCO2: PA8 AF0
*/
    *RCC_AHB4ENR |= (0x1u<<GPIOCEN)|(0x1u<<GPIOAEN);

    *GPIOA_MODER &= ~(0x3u<<MODER8);
    *GPIOA_MODER |= (0x2u<<MODER8);
    *GPIOA_AFRH &= ~(0xfu<<AFR8);

    *GPIOC_MODER &= ~(0x3u<<MODER9);
    *GPIOC_MODER |= (0x2u<<MODER9);
    *GPIOC_AFRH &= ~(0xfu<<AFR9);
    *GPIOC_OSPEEDR&= ~(0x3u<<OSPEED9);
    *GPIOC_OSPEEDR|= (0x1u<<OSPEED9);

}

int main(void)
{
    /* 2.6MHz on PC9, 48MHz on PA8 */
    init_mco(); /* init *before* enabling external oscillators and PLLs */
    init_clock();
}


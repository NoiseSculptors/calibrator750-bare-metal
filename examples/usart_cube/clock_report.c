
#include "rcc.h"
#include "init.h"
#include "printf.h"
#include "usart1.h"
#include "gpio.h"
#include "timer.h"

/******************************************************************************
  PB10/TIM2_CH3
  PB11/TIM2_CH4
******************************************************************************/

int main(void)
{
    clock_info_t ci;
    ci = init_clock();
    init_usart1(ci);

    printf("\e[?25l"); /* turn off cursor */
    printf("\033[2J\033[H"); /* clear screen */

    printf("\n===========================\n");

    printf(" \
hse_hz =         %d\n \
pll1_vco_hz =   %d\n \
pll1_p_hz =     %d\n \
pll1_q_hz =      %d\n \
pll1_r_hz =     %d\n \
sysclk_hz =     %d\n \
hclk_hz =       %d\n \
pclk1_apb1_hz = %d\n \
pclk2_apb2_hz = %d\n \
pclk3_apb3_hz = %d\n \
pclk4_apb4_hz = %d\n \
tim_apb1_hz =   %d\n \
tim_apb2_hz =   %d\n",
    ci.hse_hz,
    ci.pll1_vco_hz,
    ci.pll1_p_hz,
    ci.pll1_q_hz,
    ci.pll1_r_hz,
    ci.sysclk_hz,
    ci.hclk_hz,
    ci.pclk1_apb1_hz,
    ci.pclk2_apb2_hz,
    ci.pclk3_apb3_hz,
    ci.pclk4_apb4_hz,
    ci.tim_apb1_hz,
    ci.tim_apb2_hz);
    printf("===========================");

}

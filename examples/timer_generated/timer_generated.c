
#include "rcc.h"
#include "init.h"
#include "gpio.h"
#include "timer.h"
#include "tim_config.h"


/******************************************************************************
  PB10/TIM2_CH3
  PB11/TIM2_CH4
******************************************************************************/

/* Signal output on PB10 and PB11. */

int main(void)
{
    init_clock();

#define GPIOBEN 1
    *RCC_AHB4ENR |= (1<<GPIOBEN);

    /*
        OSPEEDR controls edge speed,
        lower speed: less ringing, less accurate duty cycle
        higher speed: more ringing, more accurate duty cycle
        ex - 80MHz 0b00 not working
        00: Low speed
        01: Medium speed
        10: High speed
        11: Very high speed
     */
    *GPIOB_OSPEEDR &= ~((0x3u<<OSPEED11) | (0x3u<<OSPEED10));
    *GPIOB_OSPEEDR |= (0x0u<<OSPEED11) | (0x0u<<OSPEED10);

    *GPIOB_MODER   &= ~((0x3u << MODER11)|(0x3u << MODER10));
    *GPIOB_MODER   |=  ((0x2u << MODER11)|(0x2u << MODER10));

#define AFR11 12
#define AFR10 8
    /* alternate function mode outputs */
    *GPIOB_AFRH &= ~((0xfu<<AFR11) | (0xfu<<AFR10));
    *GPIOB_AFRH |= (0x1u<<AFR11) | (0x1u<<AFR10);

#define MODER11 22
#define MODER10 20
    /* alternate function mode */
    *GPIOB_MODER &= ~((0x3u<<MODER11) | (0x3u<<MODER10));
    *GPIOB_MODER |= (0x2u<<MODER11) | (0x2u<<MODER10);

#define TIM2EN 0
    *RCC_APB1LENR |= (0x1u<<TIM2EN);

#define TIM1EN 0
    *RCC_APB2ENR |= (0x1u<<TIM1EN);

    *TIM2_PSC = TIM_PSC;
    *TIM2_ARR = TIM_ARR;
    *TIM2_CCR3 = TIM_CCR3;
    *TIM2_CCR4 = TIM_CCR4;

#define OC4M 12 
#define OC3M 4
#define OC2M 12 
#define OC1M 4
/*
    0110: PWM mode 1 - In upcounting, channel 1 is active as long as
    TIMx_CNT<TIMx_CCR1 else inactive. In downcounting, channel 1 is inactive
    (OC1REF=‘0) as long as TIMx_CNT>TIMx_CCR1 else active (OC1REF=1)
*/
    *TIM2_CCMR2 &= ~((0x7u<<OC4M)|(0x7u<<OC3M)); 
    *TIM2_CCMR2 |= (0x6u<<OC4M)|(0x6u<<OC3M); 

#define CC3E 8
#define CC4E 12 
    *TIM2_CCER |= (0x1u<<CC4E)|(0x1u<<CC3E);

#define CEN 0
    *TIM2_CR1 |= (0x1u<<CEN);

    while(1){
    }

    return 0;
}

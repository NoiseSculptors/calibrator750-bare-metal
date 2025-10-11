
#include "delay.h"
#include "gpio.h"
#include "init.h"
#include "rcc.h"
#include <stdint.h>

void sleep(unsigned int n);

int main(void) {

    init_clock();

    /* example for blinking LEDs on D0 D1 C10 */

    /* Enables GPIOC GPIOD peripheral clock */
    *RCC_AHB4ENR |= (0x1u<<GPIODEN) | (0x1u<<GPIOCEN);

    /* Clear MODER bits for exposed pins */
    *GPIOC_MODER &= ~(0x3u<<MODER10);
    *GPIOC_MODER |= (0x1u<<MODER10);

    *GPIOD_MODER &= ~((0x3u<<MODER1) | (0x3u<<MODER0));
    *GPIOD_MODER |= (0x1u<<MODER1) | (0x1u<<MODER0);

    while (1) {
        *GPIOD_ODR ^= (0x1u<<ODR1);
        delay_ms(200);
        *GPIOD_ODR ^= (0x1u<<ODR0);
        delay_ms(200);
        *GPIOC_ODR ^= (0x1u<<ODR10);
        delay_ms(200);
    }
}


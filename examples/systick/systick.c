
#include "init.h"
#include "systick.h"
#include "printf.h"
#include "user_io.h"
#include "nvic.h"
#include <stdint.h>

extern clock_info_t ci;

void SysTick_Handler(void)
{
    static uint32_t counter;

    if(counter++ >= 1000-1){ // 1Hz
        user_led_toggle(1);
        counter=0;
    }
}

int main(void)
{
    init_clock();
    user_led_init();
    init_systick_1ms();
}


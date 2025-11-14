
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

static void init_systick(void)
{
    uint32_t systick_clk = ci.hclk_hz / 8u;
    uint32_t reload = (systick_clk / 1000u) - 1u; // 1kHz tick

    *SYST_RVR = reload & 0x00FFFFFFu; // clamp to 24-bit
    *SYST_CVR = 0; // clear current
    *SYST_CSR = (1<<SYST_CSR_CLKSOURCE) |
                (1<<SYST_CSR_TICKINT) |
                (1<<SYST_CSR_ENABLE);
}

int main(void)
{
    init_clock();
    user_led_init();
    init_systick();
}


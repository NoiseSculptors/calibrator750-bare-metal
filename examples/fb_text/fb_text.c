
#include "delay.h"
#include "init.h"
#include "nvic.h"
#include "systick.h"
#include "user_io.h"

#define WIDTH 128
#define HEIGHT 64

void SysTick_Handler(void)
{
    user_systick_service(); // needed for framebuffer refresh
}

int main(void)
{
    init_clock();
    user_display_init();
    init_systick_1ms();

    const char text[] = {
        "              "
        " user defined "
        " display in   "
        " user_io.c    "
        " 14x8 chars   "
        "              "
        "12345678901234"
        "--------------"
    };

    fb_printf(0,0,"%s", text);

    return 0;
}


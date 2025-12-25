
#include "rcc.h"
#include "gpio.h"
#include <stdint.h>

#define AIRCR (volatile uint32_t *)0xE000ED0C
#define SYSRESETREQ 2

int main(void)
{
    /* 0x05fa<<16 has to be written, otherwise 1<<SYSRESETREQ is ignored */
    *AIRCR = (0x05fa<<16) | (1<<SYSRESETREQ);
    while(1);
    return 0;
}



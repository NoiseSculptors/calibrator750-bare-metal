
#include "delay.h"
#include <stdint.h>

/*
   warning:
   VERY basic, NOT completely accurate!!!
 */

void tiny_delay(uint32_t n);

__attribute__((optimize(0)))
void delay_ms(uint32_t ms)
{
    while(ms--)
        tiny_delay(10000);
    
}

__attribute__((optimize(0)))
void delay_us(uint32_t us)
{
    switch(us){
        case(1):
            for(int i=0;i<18;i++)
                __asm__ volatile("nop");
            break;
        case(2):
            for(int i=0;i<110;i++)
                __asm__ volatile("nop");
            break;
        default:
            while(us--){
                for(int i=0;i<56;i++)
                    __asm__ volatile("nop");
            }
            break;
    }
}

void tiny_delay(uint32_t n)
{
    for (volatile unsigned i=0; i<n; i++) __asm__ volatile("nop");
}

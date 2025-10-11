#ifndef RCC_H
#define RCC_H

#include "memory.h"

#define GPIOAEN 0
#define GPIOBEN 1
#define GPIOCEN 2
#define GPIODEN 3
#define GPIOEEN 4
#define GPIOFEN 5
#define GPIOGEN 6
#define GPIOHEN 7
#define GPIOIEN 8
#define GPIOJEN 9
#define GPIOKEN 10

#define RCC_CR           (volatile unsigned int *)(RCC + 0x00)
#define RCC_ICSCR        (volatile unsigned int *)(RCC + 0x04)
#define RCC_HSICFGR      (volatile unsigned int *)(RCC + 0x04)
#define RCC_CRRCR        (volatile unsigned int *)(RCC + 0x08)
#define RCC_CSICFGR      (volatile unsigned int *)(RCC + 0x0C)
#define RCC_CFGR         (volatile unsigned int *)(RCC + 0x10)
#define RCC_D1CFGR       (volatile unsigned int *)(RCC + 0x18)
#define RCC_D2CFGR       (volatile unsigned int *)(RCC + 0x1C)
#define RCC_D3CFGR       (volatile unsigned int *)(RCC + 0x20)
#define RCC_PLLCKSELR    (volatile unsigned int *)(RCC + 0x28)
#define RCC_PLLCFGR      (volatile unsigned int *)(RCC + 0x2c)
#define RCC_PLL1DIVR     (volatile unsigned int *)(RCC + 0x30)
#define RCC_PLL1FRACR    (volatile unsigned int *)(RCC + 0x34)
#define RCC_PLL2DIVR     (volatile unsigned int *)(RCC + 0x38)
#define RCC_PLL2FRACR    (volatile unsigned int *)(RCC + 0x3c)
#define RCC_PLL3DIVR     (volatile unsigned int *)(RCC + 0x40)
#define RCC_PLL3FRACR    (volatile unsigned int *)(RCC + 0x44)
#define RCC_D1CCIPR      (volatile unsigned int *)(RCC + 0x4c)
#define RCC_D2CCIP1R     (volatile unsigned int *)(RCC + 0x50)
#define RCC_D2CCIP2R     (volatile unsigned int *)(RCC + 0x54)
#define RCC_D3CCIPR      (volatile unsigned int *)(RCC + 0x58)
#define RCC_CIER         (volatile unsigned int *)(RCC + 0x60)
#define RCC_CIFR         (volatile unsigned int *)(RCC + 0x64)
#define RCC_CICR         (volatile unsigned int *)(RCC + 0x68)
#define RCC_BDCR         (volatile unsigned int *)(RCC + 0x70)
#define RCC_CSR          (volatile unsigned int *)(RCC + 0x74)
#define RCC_AHB3RSTR     (volatile unsigned int *)(RCC + 0x7c)
#define RCC_AHB1RSTR     (volatile unsigned int *)(RCC + 0x80)
#define RCC_AHB2RSTR     (volatile unsigned int *)(RCC + 0x84)
#define RCC_AHB4RSTR     (volatile unsigned int *)(RCC + 0x88)
#define RCC_APB3RSTR     (volatile unsigned int *)(RCC + 0x8c)
#define RCC_APB1LRSTR    (volatile unsigned int *)(RCC + 0x90)
#define RCC_APB1HRSTR    (volatile unsigned int *)(RCC + 0x94)
#define RCC_APB2RSTR     (volatile unsigned int *)(RCC + 0x98)
#define RCC_APB4RSTR     (volatile unsigned int *)(RCC + 0x9c)
#define RCC_GCR          (volatile unsigned int *)(RCC + 0xa0)
#define RCC_D3AMR        (volatile unsigned int *)(RCC + 0xa8)
#define RCC_RSR          (volatile unsigned int *)(RCC + 0xd0)
#define RCC_AHB3ENR      (volatile unsigned int *)(RCC + 0xd4)
#define RCC_AHB1ENR      (volatile unsigned int *)(RCC + 0xd8)
#define RCC_AHB2ENR      (volatile unsigned int *)(RCC + 0xdc)
#define RCC_AHB4ENR      (volatile unsigned int *)(RCC + 0xe0)
#define RCC_APB3ENR      (volatile unsigned int *)(RCC + 0xe4)
#define RCC_APB1LENR     (volatile unsigned int *)(RCC + 0xe8)
#define RCC_APB1HENR     (volatile unsigned int *)(RCC + 0xec)
#define RCC_APB2ENR      (volatile unsigned int *)(RCC + 0xf0)
#define RCC_APB4ENR      (volatile unsigned int *)(RCC + 0xf4)
#define RCC_AHB3LPENR    (volatile unsigned int *)(RCC + 0xfc)
#define RCC_AHB1LPENR    (volatile unsigned int *)(RCC + 0x100)
#define RCC_AHB2LPENR    (volatile unsigned int *)(RCC + 0x104)
#define RCC_AHB4LPENR    (volatile unsigned int *)(RCC + 0x108)
#define RCC_APB3LPENR    (volatile unsigned int *)(RCC + 0x10c)
#define RCC_APB1LLPENR   (volatile unsigned int *)(RCC + 0x110)
#define RCC_APB1HLPENR   (volatile unsigned int *)(RCC + 0x114)
#define RCC_APB2LPENR    (volatile unsigned int *)(RCC + 0x118)
#define RCC_APB4LPENR    (volatile unsigned int *)(RCC + 0x11c)
#define RCC_C1_AHB3ENR   (volatile unsigned int *)(RCC + 0x134)
#define RCC_C1_AHB1ENR   (volatile unsigned int *)(RCC + 0x138)
#define RCC_C1_AHB2ENR   (volatile unsigned int *)(RCC + 0x13c)
#define RCC_C1_AHB4ENR   (volatile unsigned int *)(RCC + 0x140)
#define RCC_C1_APB3ENR   (volatile unsigned int *)(RCC + 0x144)
#define RCC_C1_APB1LENR  (volatile unsigned int *)(RCC + 0x148)
#define RCC_C1_APB1HENR  (volatile unsigned int *)(RCC + 0x14c)
#define RCC_C1_APB2ENR   (volatile unsigned int *)(RCC + 0x150)
/* .. etc :) */

#endif

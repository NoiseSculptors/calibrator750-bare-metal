#ifndef GPIO_h
#define GPIO_h

#include "memory.h"

#define MODER15  30
#define MODER14  28
#define MODER13  26
#define MODER12  24
#define MODER11  22
#define MODER10  20
#define MODER9   18
#define MODER8   16
#define MODER7   14
#define MODER6   12
#define MODER5   10
#define MODER4    8
#define MODER3    6
#define MODER2    4
#define MODER1    2
#define MODER0    0

#define OSPEED15  30
#define OSPEED14  28
#define OSPEED13  26
#define OSPEED12  24
#define OSPEED11  22
#define OSPEED10  20
#define OSPEED9   18
#define OSPEED8   16
#define OSPEED7   14
#define OSPEED6   12
#define OSPEED5   10
#define OSPEED4    8
#define OSPEED3    6
#define OSPEED2    4
#define OSPEED1    2
#define OSPEED0    0

#define PUPDR15  30
#define PUPDR14  28
#define PUPDR13  26
#define PUPDR12  24
#define PUPDR11  22
#define PUPDR10  20
#define PUPDR9   18
#define PUPDR8   16
#define PUPDR7   14
#define PUPDR6   12
#define PUPDR5   10
#define PUPDR4    8
#define PUPDR3    6
#define PUPDR2    4
#define PUPDR1    2
#define PUPDR0    0

#define OSPEED15  30
#define OSPEED14  28
#define OSPEED13  26
#define OSPEED12  24
#define OSPEED11  22
#define OSPEED10  20
#define OSPEED9   18
#define OSPEED8   16
#define OSPEED7   14
#define OSPEED6   12
#define OSPEED5   10
#define OSPEED4    8
#define OSPEED3    6
#define OSPEED2    4
#define OSPEED1    2
#define OSPEED0    0

#define ODR15  15
#define ODR14  14
#define ODR13  13
#define ODR12  12
#define ODR11  11
#define ODR10  10
#define ODR9   9 
#define ODR8   8 
#define ODR7   7 
#define ODR6   6 
#define ODR5   5 
#define ODR4   4 
#define ODR3   3 
#define ODR2   2 
#define ODR1   1 
#define ODR0   0 

#define OT15  15
#define OT14  14
#define OT13  13
#define OT12  12
#define OT11  11
#define OT10  10
#define OT9   9 
#define OT8   8 
#define OT7   7 
#define OT6   6 
#define OT5   5 
#define OT4   4 
#define OT3   3 
#define OT2   2 
#define OT1   1 
#define OT0   0 

#define AFR15  28
#define AFR14  24
#define AFR13  20
#define AFR12  16
#define AFR11  12
#define AFR10  8
#define AFR9   4 
#define AFR8   0
#define AFR7   28
#define AFR6   24
#define AFR5   20
#define AFR4   16 
#define AFR3   12 
#define AFR2   8 
#define AFR1   4 
#define AFR0   0 

#define IDR15  15
#define IDR14  14
#define IDR13  13
#define IDR12  12
#define IDR11  11
#define IDR10  10
#define IDR9   9 
#define IDR8   8 
#define IDR7   7 
#define IDR6   6 
#define IDR5   5 
#define IDR4   4 
#define IDR3   3 
#define IDR2   2 
#define IDR1   1 
#define IDR0   0 

#define GPIOA_MODER      (volatile unsigned int *)(GPIOA + 0x00)
#define GPIOA_OTYPER     (volatile unsigned int *)(GPIOA + 0x04)
#define GPIOA_OSPEEDR    (volatile unsigned int *)(GPIOA + 0x08)
#define GPIOA_PUPDR      (volatile unsigned int *)(GPIOA + 0x0c)
#define GPIOA_IDR        (volatile unsigned int *)(GPIOA + 0x10)
#define GPIOA_ODR        (volatile unsigned int *)(GPIOA + 0x14)
#define GPIOA_BSRR       (volatile unsigned int *)(GPIOA + 0x18)
#define GPIOA_LCKR       (volatile unsigned int *)(GPIOA + 0x1c)
#define GPIOA_AFRL       (volatile unsigned int *)(GPIOA + 0x20)
#define GPIOA_AFRH       (volatile unsigned int *)(GPIOA + 0x24)

#define GPIOB_MODER      (volatile unsigned int *)(GPIOB + 0x00)
#define GPIOB_OTYPER     (volatile unsigned int *)(GPIOB + 0x04)
#define GPIOB_OSPEEDR    (volatile unsigned int *)(GPIOB + 0x08)
#define GPIOB_PUPDR      (volatile unsigned int *)(GPIOB + 0x0c)
#define GPIOB_IDR        (volatile unsigned int *)(GPIOB + 0x10)
#define GPIOB_ODR        (volatile unsigned int *)(GPIOB + 0x14)
#define GPIOB_BSRR       (volatile unsigned int *)(GPIOB + 0x18)
#define GPIOB_LCKR       (volatile unsigned int *)(GPIOB + 0x1c)
#define GPIOB_AFRL       (volatile unsigned int *)(GPIOB + 0x20)
#define GPIOB_AFRH       (volatile unsigned int *)(GPIOB + 0x24)

#define GPIOC_MODER      (volatile unsigned int *)(GPIOC + 0x00)
#define GPIOC_OTYPER     (volatile unsigned int *)(GPIOC + 0x04)
#define GPIOC_OSPEEDR    (volatile unsigned int *)(GPIOC + 0x08)
#define GPIOC_PUPDR      (volatile unsigned int *)(GPIOC + 0x0c)
#define GPIOC_IDR        (volatile unsigned int *)(GPIOC + 0x10)
#define GPIOC_ODR        (volatile unsigned int *)(GPIOC + 0x14)
#define GPIOC_BSRR       (volatile unsigned int *)(GPIOC + 0x18)
#define GPIOC_LCKR       (volatile unsigned int *)(GPIOC + 0x1c)
#define GPIOC_AFRL       (volatile unsigned int *)(GPIOC + 0x20)
#define GPIOC_AFRH       (volatile unsigned int *)(GPIOC + 0x24)

#define GPIOD_MODER      (volatile unsigned int *)(GPIOD + 0x00)
#define GPIOD_OTYPER     (volatile unsigned int *)(GPIOD + 0x04)
#define GPIOD_OSPEEDR    (volatile unsigned int *)(GPIOD + 0x08)
#define GPIOD_PUPDR      (volatile unsigned int *)(GPIOD + 0x0c)
#define GPIOD_IDR        (volatile unsigned int *)(GPIOD + 0x10)
#define GPIOD_ODR        (volatile unsigned int *)(GPIOD + 0x14)
#define GPIOD_BSRR       (volatile unsigned int *)(GPIOD + 0x18)
#define GPIOD_LCKR       (volatile unsigned int *)(GPIOD + 0x1c)
#define GPIOD_AFRL       (volatile unsigned int *)(GPIOD + 0x20)
#define GPIOD_AFRH       (volatile unsigned int *)(GPIOD + 0x24)

#define GPIOE_MODER      (volatile unsigned int *)(GPIOE + 0x00)
#define GPIOE_OTYPER     (volatile unsigned int *)(GPIOE + 0x04)
#define GPIOE_OSPEEDR    (volatile unsigned int *)(GPIOE + 0x08)
#define GPIOE_PUPDR      (volatile unsigned int *)(GPIOE + 0x0c)
#define GPIOE_IDR        (volatile unsigned int *)(GPIOE + 0x10)
#define GPIOE_ODR        (volatile unsigned int *)(GPIOE + 0x14)
#define GPIOE_BSRR       (volatile unsigned int *)(GPIOE + 0x18)
#define GPIOE_LCKR       (volatile unsigned int *)(GPIOE + 0x1c)
#define GPIOE_AFRL       (volatile unsigned int *)(GPIOE + 0x20)
#define GPIOE_AFRH       (volatile unsigned int *)(GPIOE + 0x24)

#endif

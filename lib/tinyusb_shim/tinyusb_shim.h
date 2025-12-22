#ifndef TINYUSB_SHIM_H
#define TINYUSB_SHIM_H

#include "nvic.h"
#include "nvic_exti.h"
#include "memory.h"
#include <stdint.h>


#define USB_OTG_FS_PERIPH_BASE   USB2_OTG_FS
#define NVIC_DisableIRQ(i)       irq_disable(i)
#define NVIC_EnableIRQ(i)        irq_enable(i)
#define CFG_TUSB_MCU             OPT_MCU_STM32H7
#define __CORTEX_M               7
#define __NOP()                  __asm__("nop");
#define  OTG_FS_IRQn             IRQ_OTG_FS

extern uint32_t SystemCoreClock;

typedef int32_t IRQn_Type;

int board_getchar(void);
void board_putchar(int c);

#endif

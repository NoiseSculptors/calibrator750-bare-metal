#ifndef USART_UART_H
#define USART_UART_H

#include "memory.h"

#define USART1_CR1      (volatile unsigned int *)(USART1 + 0x00)
#define USART1_CR2      (volatile unsigned int *)(USART1 + 0x04)
#define USART1_CR3      (volatile unsigned int *)(USART1 + 0x08)
#define USART1_BRR      (volatile unsigned int *)(USART1 + 0x0C)
#define USART1_ISR      (volatile unsigned int *)(USART1 + 0x1C)
#define USART1_RDR      (volatile unsigned int *)(USART1 + 0x24)
#define USART1_TDR      (volatile unsigned int *)(USART1 + 0x28)

#define USART2_CR1      (volatile unsigned int *)(USART2 + 0x00)
#define USART2_CR2      (volatile unsigned int *)(USART2 + 0x04)
#define USART2_CR3      (volatile unsigned int *)(USART2 + 0x08)
#define USART2_BRR      (volatile unsigned int *)(USART2 + 0x0C)
#define USART2_ISR      (volatile unsigned int *)(USART2 + 0x1C)
#define USART2_RDR      (volatile unsigned int *)(USART2 + 0x24)
#define USART2_TDR      (volatile unsigned int *)(USART2 + 0x28)

#define USART3_CR1      (volatile unsigned int *)(USART3 + 0x00)
#define USART3_CR2      (volatile unsigned int *)(USART3 + 0x04)
#define USART3_CR3      (volatile unsigned int *)(USART3 + 0x08)
#define USART3_BRR      (volatile unsigned int *)(USART3 + 0x0C)
#define USART3_ISR      (volatile unsigned int *)(USART3 + 0x1C)
#define USART3_RDR      (volatile unsigned int *)(USART3 + 0x24)
#define USART3_TDR      (volatile unsigned int *)(USART3 + 0x28)

#endif

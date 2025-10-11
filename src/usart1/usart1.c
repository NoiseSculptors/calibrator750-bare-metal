#include "gpio.h"
#include "rcc.h"
#include "init.h"
#include "usart1.h"
#include "usart_uart.h"
#include <stdint.h>

/* configures USART1 on
   AF7, 
   PA9  USART1_TX
   PA10 USART1_RX
 */

void init_usart1(clock_info_t ci)
{

#define USART1EN 4
    /* Enable clock to GPIOA and USART1 */
    *RCC_AHB4ENR |= (1<<GPIOAEN);
    *RCC_APB2ENR |= (1<<USART1EN);

    /* switch to alternate function */
    *GPIOA_MODER &= ~((0b11<<MODER10) | (0b11<<MODER9));
    *GPIOA_MODER |=  ((0b10<<MODER10) | (0b10<<MODER9));

    /* set alternate function 7 */
    *GPIOA_AFRH &= ~((0b1111<<AFR10) | (0b1111<<AFR9));
    *GPIOA_AFRH |=  (7<<AFR10)|(7<<AFR9); 

    /* 4. Configure basic USART settings:
          - Disable USART before configuration
          - Word length: 8 bits
          - Parity: none
          - Stop bits: 1 (CR2 default)
          - Enable TX and RX */

    // Disable USART
#define UE 0
    *USART1_CR1 &= ~(1<<UE);

    /* For 8N1, leave M1, M0, PCE, etc. at default (0).
       TE = 1: enable transmitter
       RE = 1: enable receiver */
#define TE 3
#define RE 2
    *USART1_CR1 |= (1<<TE) | (1<<RE);

    /* Set baud rate
       BRR = APB2_CLK / Desired_Baud
       for calibration - sending 'U' continuously on 115200
       should give about 57.60kHz square signal */

    /* RCC_D2CCIP2R default clock for usart1 and usart6 is apb2 (rcc_pclk2) */
    *USART1_BRR = ci.pclk2_apb2_hz / 115200;

    // Enable USART
    *USART1_CR1 |= (1<<UE);
}

void usart1_write_char(int c)
{
    /* Wait until TXE transmit data register empty */
#define TXE 7
    while (!(*USART1_ISR & (1<<TXE))){}

    /* Load data into TDR */
    *USART1_TDR = c;
}

int usart1_read_char(void)
{

#define RXNE 5
    /* Wait until RXNE read data register not empty */
    while (!(*USART1_ISR & (1 << RXNE))){}
    // Return the received character
    return *USART1_RDR & 0xFF;
}


#include "dma.h"
#include "dmamux.h"
#include "i2c.h"
#include "rcc.h"
#include <stdint.h>

void init_dma_i2c(uint32_t periph){

    uint8_t dma_request_mux_input;

    /* dma_request_mux_input from table 123. RM0433 */
    if(periph==I2C1)
        dma_request_mux_input = 34;
    else if (periph==I2C2)
        dma_request_mux_input = 36;
    else // (periph==I2C3)
        dma_request_mux_input = 74;

    // Clocks
    *RCC_AHB1ENR |= (1u<<0); // DMA1EN

    // DMAMUX: route I2C2_TX to DMA1 Stream6
    *DMAMUX1_C6CR = 0; // clear first

    *DMAMUX1_C6CR = (dma_request_mux_input & 0x7F);

    // Make sure stream disabled
    *DMA1_S6CR &= ~1u;
    while (*DMA1_S6CR & 1u) {}

    // Addresses
    *DMA1_S6PAR  = (uint32_t)I2C_TXDR(periph); // peripheral = I2C2->TXDR

    // CR: M2P | MINC | 8/8-bit | priority | (optional) TCIE/TEIE
    *DMA1_S6CR = (1u<<10) // MINC=1
               | (1u<<6); // DIR = Memory to peripheral

    // I2C: enable TXDMA
    *I2C_CR1(periph) |= (1u<<14)  // TXDMAEN
                     | (1u<<0);   // enable
}


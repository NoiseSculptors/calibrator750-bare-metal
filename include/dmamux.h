#ifndef DMAMUX_H
#define DMAMUX_H

/*
(from ST reference manual RM0433 Rev 8)

DMAMUX1 request line multiplexer channel x configuration register (DMAMUX1_CxCR) Address offset: 0x000 + 0x04 * x (x = 0 to 15)
DMAMUX1 request line multiplexer interrupt channel status register (DMAMUX1_CSR) Address offset: 0x080
DMAMUX1 request line multiplexer interrupt clear flag register (DMAMUX1_CFR) Address offset: 0x084
DMAMUX1 request generator channel x configuration register (DMAMUX1_RGxCR) Address offset: 0x100 + 0x04 * x (x = 0 to 7)
DMAMUX1 request generator interrupt clear flag register (DMAMUX1_RGCFR) Address offset: 0x144
DMAMUX1 request generator interrupt status register (DMAMUX1_RGSR) Address offset: 0x140

DMAMUX2 request line multiplexer channel x configuration register (DMAMUX2_CxCR) Address offset: 0x000 + 0x04 * x, where x = 0 to 7
DMAMUX2 request line multiplexer interrupt channel status register (DMAMUX2_CSR) Address offset: 0x080
DMAMUX2 request line multiplexer interrupt clear flag register (DMAMUX2_CFR) Address offset: 0x084
DMAMUX2 request generator channel x configuration register (DMAMUX2_RGxCR) Address offset: 0x100 + 0x04 * x (x = 0 to 7)
DMAMUX2 request generator interrupt clear flag register (DMAMUX2_RGCFR) Address offset: 0x144
DMAMUX2 request generator interrupt status register (DMAMUX2_RGSR) Address offset: 0x140
*/

#define DMAMUX1_C0CR      (volatile unsigned int *)(DMAMUX1 + 0x000)
#define DMAMUX1_C1CR      (volatile unsigned int *)(DMAMUX1 + 0x004)
#define DMAMUX1_C2CR      (volatile unsigned int *)(DMAMUX1 + 0x008)
#define DMAMUX1_C3CR      (volatile unsigned int *)(DMAMUX1 + 0x00c)
#define DMAMUX1_C4CR      (volatile unsigned int *)(DMAMUX1 + 0x010)
#define DMAMUX1_C5CR      (volatile unsigned int *)(DMAMUX1 + 0x014)
#define DMAMUX1_C6CR      (volatile unsigned int *)(DMAMUX1 + 0x018)
#define DMAMUX1_C7CR      (volatile unsigned int *)(DMAMUX1 + 0x01c)
#define DMAMUX1_C8CR      (volatile unsigned int *)(DMAMUX1 + 0x020)
#define DMAMUX1_C9CR      (volatile unsigned int *)(DMAMUX1 + 0x024)
#define DMAMUX1_C10CR     (volatile unsigned int *)(DMAMUX1 + 0x028)
#define DMAMUX1_C11CR     (volatile unsigned int *)(DMAMUX1 + 0x02c)
#define DMAMUX1_C12CR     (volatile unsigned int *)(DMAMUX1 + 0x030)
#define DMAMUX1_C13CR     (volatile unsigned int *)(DMAMUX1 + 0x034)
#define DMAMUX1_C14CR     (volatile unsigned int *)(DMAMUX1 + 0x038)
#define DMAMUX1_C15CR     (volatile unsigned int *)(DMAMUX1 + 0x03c)
#define DMAMUX1_CSR       (volatile unsigned int *)(DMAMUX1 + 0x080)
#define DMAMUX1_CFR       (volatile unsigned int *)(DMAMUX1 + 0x084)
#define DMAMUX1_RG0CR     (volatile unsigned int *)(DMAMUX1 + 0x100)
#define DMAMUX1_RG1CR     (volatile unsigned int *)(DMAMUX1 + 0x104)
#define DMAMUX1_RG2CR     (volatile unsigned int *)(DMAMUX1 + 0x108)
#define DMAMUX1_RG3CR     (volatile unsigned int *)(DMAMUX1 + 0x10c)
#define DMAMUX1_RGSR      (volatile unsigned int *)(DMAMUX1 + 0x140)
#define DMAMUX1_RGCFR     (volatile unsigned int *)(DMAMUX1 + 0x144)

#define DMAMUX2_C0CR      (volatile unsigned int *)(DMAMUX2 + 0x000)
#define DMAMUX2_C1CR      (volatile unsigned int *)(DMAMUX2 + 0x004)
#define DMAMUX2_C2CR      (volatile unsigned int *)(DMAMUX2 + 0x008)
#define DMAMUX2_C3CR      (volatile unsigned int *)(DMAMUX2 + 0x00c)
#define DMAMUX2_C4CR      (volatile unsigned int *)(DMAMUX2 + 0x010)
#define DMAMUX2_C5CR      (volatile unsigned int *)(DMAMUX2 + 0x014)
#define DMAMUX2_C6CR      (volatile unsigned int *)(DMAMUX2 + 0x018)
#define DMAMUX2_C7CR      (volatile unsigned int *)(DMAMUX2 + 0x01c)
#define DMAMUX2_C8CR      (volatile unsigned int *)(DMAMUX2 + 0x020)
#define DMAMUX2_C9CR      (volatile unsigned int *)(DMAMUX2 + 0x024)
#define DMAMUX2_C10CR     (volatile unsigned int *)(DMAMUX2 + 0x028)
#define DMAMUX2_C11CR     (volatile unsigned int *)(DMAMUX2 + 0x02c)
#define DMAMUX2_C12CR     (volatile unsigned int *)(DMAMUX2 + 0x030)
#define DMAMUX2_C13CR     (volatile unsigned int *)(DMAMUX2 + 0x034)
#define DMAMUX2_C14CR     (volatile unsigned int *)(DMAMUX2 + 0x038)
#define DMAMUX2_C15CR     (volatile unsigned int *)(DMAMUX2 + 0x03c)
#define DMAMUX2_CSR       (volatile unsigned int *)(DMAMUX2 + 0x080)
#define DMAMUX2_CFR       (volatile unsigned int *)(DMAMUX2 + 0x084)
#define DMAMUX2_RG0CR     (volatile unsigned int *)(DMAMUX2 + 0x100)
#define DMAMUX2_RG1CR     (volatile unsigned int *)(DMAMUX2 + 0x104)
#define DMAMUX2_RG2CR     (volatile unsigned int *)(DMAMUX2 + 0x108)
#define DMAMUX2_RG3CR     (volatile unsigned int *)(DMAMUX2 + 0x10c)
#define DMAMUX2_RGSR      (volatile unsigned int *)(DMAMUX2 + 0x140)
#define DMAMUX2_RGCFR     (volatile unsigned int *)(DMAMUX2 + 0x144)

#endif

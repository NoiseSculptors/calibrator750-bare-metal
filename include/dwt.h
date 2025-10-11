#ifndef DWT_H
#define DWT_H

#define DWT_CTRL    (volatile uint32_t *)(0xE0001000)
#define DWT_CYCCNT  (volatile uint32_t *)(0xE0001004)
#define DEMCR       (volatile uint32_t *)(0xE000EDFC)

/*
    count cpu cycles of a call

    *DWT_CYCCNT = 0; // reset counter
    a_function_to_measure();
    uint32_t cycles = DWT_CYCCNT;
    xprintf("Cycles measured: %d\n", cycles);

 */

void init_DWT(void);

#endif

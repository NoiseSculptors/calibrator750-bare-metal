#ifndef PWR_H
#define PWR_H

#include "memory.h"

#define PWR_CSR1      (volatile unsigned int *)(PWR + 0x004)
#define PWR_CR3       (volatile unsigned int *)(PWR + 0x00c)
#define PWR_D3CR      (volatile unsigned int *)(PWR + 0x018)

#endif

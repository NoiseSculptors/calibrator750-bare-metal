
#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>

#define NVIC_ISER_BASE (0xE000E100U)
#define NVIC_ISER ((volatile uint32_t*) NVIC_ISER_BASE)

void NVIC_EnableIRQ(int32_t IRQn);

#endif


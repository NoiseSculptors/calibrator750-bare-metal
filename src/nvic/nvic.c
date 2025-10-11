
#include "nvic.h"
#include <stdint.h>

void NVIC_EnableIRQ(int32_t IRQn) {
    if (IRQn >= 0) {
        NVIC_ISER[IRQn >> 5] = (1U << (IRQn & 0x1F));
    }
}


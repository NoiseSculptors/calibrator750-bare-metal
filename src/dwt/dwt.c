#include "dwt.h"
#include <stdint.h>

void init_DWT(void) {
   *DEMCR |= (1 << 24);     // Enable trace and debug block
   *DWT_CTRL |= 1;          // Enable cycle counter
   *DWT_CYCCNT = 0;         // Reset cycle counter
}


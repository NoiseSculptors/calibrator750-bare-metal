#ifndef SPI_I2S_h
#define SPI_I2S_h

#include "memory.h"

#define SPI3_CR1           (volatile unsigned int*)(SPI3 + 0x00)
#define SPI3_CFG1          (volatile unsigned int*)(SPI3 + 0x08)
#define SPI3_SR            (volatile unsigned int*)(SPI3 + 0x14)
#define SPI3_TXDR          (volatile unsigned int*)(SPI3 + 0x20)
#define SPI3_I2SCFGR       (volatile unsigned int*)(SPI3 + 0x50)

#endif

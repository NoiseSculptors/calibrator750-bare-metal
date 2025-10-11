
/*
STM32H750 Memory Map Overview
Memory Region   Start Address   Size
ITCM RAM        0x00000000       64 KB         Closest to core, zero wait state, ideal for time-critical code.
DTCM RAM        0x20000000      128 KB         Also zero-wait, tightly coupled, best for critical data, stack.
AXI SRAM        0x24000000      512 KB         Main RAM, on AXI bus. DMA-accessible, moderate latency.
SRAM1           0x30000000      128 KB         On D1 domain, good general-purpose RAM.
SRAM2           0x30020000      128 KB         D1 domain again, same as SRAM1.
SRAM3           0x30040000       32 KB         D1 domain. Smaller but similar use.
SRAM4           0x38000000       64 KB         On D3 domain (slower), good for low-priority data.
Backup SRAM     0x38800000        4 KB         Retained in Standby mode.
FLASH (main)    0x08000000      128 KB         Flash
System Memory   0x1FF00000                     Boot ROM, ST bootloader.
*/

MEMORY
{
  ITCM  (rwx)   : ORIGIN = 0x00000000, LENGTH =  64K
  FLASH (rx)    : ORIGIN = 0x08000000, LENGTH = 128K
  DTCM  (rwx)   : ORIGIN = 0x20000000, LENGTH = 128K
  AXI   (rwx)   : ORIGIN = 0x24000000, LENGTH = 512K
  SRAM1 (rwx)   : ORIGIN = 0x30000000, LENGTH = 128K
  SRAM2 (rwx)   : ORIGIN = 0x30020000, LENGTH = 128K
  SRAM3 (rwx)   : ORIGIN = 0x30040000, LENGTH =  32K
  SRAM4 (rwx)   : ORIGIN = 0x38000000, LENGTH =  64K
  BKPSRAM (rwx) : ORIGIN = 0x38800000, LENGTH =   4K
  QUADSPI (rx)  : ORIGIN = 0x90000000, LENGTH =  16M
}

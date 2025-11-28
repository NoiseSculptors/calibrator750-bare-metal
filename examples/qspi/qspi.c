
#include "dwt.h"
#include "init.h"
#include "printf.h"
#include "qspi.h"
#include "serial.h"
#include "serial.h"
#include "user_io.h"

#define QSPI_BASE    0x90000000UL
#define QSPI_SIZE    (16 * 1024 * 1024UL)

extern clock_info_t ci;

int main(void)
{
    init_clock();
    user_serial_init();
    init_dwt();



    /*
    init_qspi_80MHz();
    */

    /* or */

    /*
    104MHz qspi (max allowed for the flash chip), PLL2 as clock source
    thus consuming more energy
     */

    init_qspi_pll2_104MHz();



    /*
      Read/Write test
    */
    printf("\033[2J\033[H"); /* clear screen */

    int32_t size = 0x100;
    uint8_t buf8[size*4];

    uint32_t offset = (4*1024*123); /* you can adjust to test different offsets
                                   (from the beginning of the qspi chip) */

    for(int i=0;i<=size*4;i+=1)
        buf8[i] = i;

    printf("Data to write\n");
    hexdump((uint32_t)(&buf8),size);
    printf("\n");

    qspi_write_flash_qpi(offset, (uint32_t*)buf8, size);

    qspi_read(offset, (uint32_t*)buf8, size);

    qspi_memory_map_mode();

    printf("Reading in memory mapped mode\n");
    hexdump(QSPI_BASE + offset, size);
    printf("\n");

    printf("Reading with qsp_read function\n");
    hexdump((uint32_t)(&buf8),size);
    printf("\n");



    /*
       Flash speed test
    */
    volatile uint32_t *p32 = (volatile uint32_t *)QSPI_BASE;
    uint32_t words = QSPI_SIZE / 4;
    uint32_t sum32 = 0;

    dwt_start();

    for (uint32_t i = 0; i < words; i++) {
        sum32 ^= p32[i];   /* 32-bit reads */
    }

    uint32_t cycles = dwt_stop();

    /* keep it so compiler doesn't kill the loop */
    printf("\nsum32   = 0x%08lx\n", (unsigned long)sum32);

    double seconds  = (double)cycles / (double)ci.sysclk_hz;
    double MBps     = (double)QSPI_SIZE / (double)(1024.0*1024.0) / seconds;

    printf("\nThe whole flash chip, %dMB read done in %.2f seconds.",
            QSPI_SIZE/1024/1024, seconds);
    printf("\nTransfer rate %.2f MB/s\n", MBps);
}



#include "dwt.h"
#include "init.h"
#include "printf.h"
#include "qspi.h"
#include "serial.h"
#include "serial.h"
#include "user_io.h"

/* do not use 0 as QSPI_CLK_MULTIPLIER as it hangs the chip, possible described
here, possibly hangs when using other clock soruces than QSPISEL:0
 https://www.st.com/resource/en/errata_sheet/es0445-stm32h745xig-stm32h755xi-stm32h747xig-stm32h757xi-device-errata-stmicroelectronics.pdf
 */
#define QSPI_CLK_MULTIPLIER 2U /* 2: Fquadspi_ker_ck/3; 240/3= 80MHz */

#if defined(QSPI_PRESCALER) && (QSPI_PRESCALER == 0)
#error "QSPI prescaler 0 is forbidden due to STM32H7 QUADSPI errata"
#endif

#define QSPI_BASE           0x90000000UL
#define QSPI_SIZE           (16 * 1024 * 1024UL)

extern clock_info_t ci;

int main(void)
{
    init_clock();
    init_dwt();
    user_serial_init();

    /* 80MHz qspi */
    init_qspi(QSPI_CLK_MULTIPLIER);

    /* or */

    /* 104MHz qspi (max allowed for the flash chip), PLL2 as clock source */
//  init_qspi_pll2_clock_source_qspi_104MHz();

    qspi_memory_map_mode();

    printf("\033[2J\033[H"); /* clear screen */
    hexdump(QSPI_BASE,0x100);

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


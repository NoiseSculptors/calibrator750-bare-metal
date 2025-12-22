
#include "bsp/board_api.h"
#include "delay.h"
#include "gpio.h"
#include "dwt.h"
#include "i2c.h"
#include "init.h"
#include "printf.h"
#include "pwr.h"
#include "qspi.h"
#include "rcc.h"
#include "systick.h"
#include "tusb.h"
#include "usb.h"
#include "user_io.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define QSPI_CLK_MULTIPLIER 2U /* multiplier 2, qspi clk == 80MHz */

int main(void)
{
    init_clock();
    init_qspi_pll2_104MHz();
    main_tinyusb();

    /* never reached */

    return 0;
}


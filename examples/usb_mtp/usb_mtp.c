
#include "bsp/board_api.h"
#include "delay.h"
#include "gpio.h"
#include "dwt.h"
#include "i2c.h"
#include "init.h"
#include "printf.h"
#include "pwr.h"
#include "rcc.h"
#include "systick.h"
#include "tusb.h"
#include "usb.h"
#include "user_io.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

int main(void)
{
    init_clock();
    user_serial_init();

    main_tinyusb();

    /* never reached */

    return 0;
}


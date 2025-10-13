
#include "printf.h"
#include "ssd1315.h"
#include "ssd1315_UGUI.h"
#include "ugui.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#define SSD1315_PRINTF_BUFSZ 512 

int ssd1315_printf(int x, int y, const char *fmt, ...)
{
    char buf[SSD1315_PRINTF_BUFSZ];

    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    // Early out if nothing to draw
    if (buf[0] == '\0') return n;

    UG_PutString(x, y, buf);

    return n;
}



#include "printf.h"
#include "ssd1315.h"
#include "ssd1315_UGUI.h"
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

void UG_DrawPixel_SSD1315(UG_S16 x, UG_S16 y, UG_COLOR c)
{
    // uGUI uses 16-bit RGB565 "color"; treat nonzero as ON
    ssd1315_putpix((int)x, (int)y, c==0?C_BLACK:C_WHITE);
}


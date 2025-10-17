
#ifndef SSD1315_UGUI_H
#define SSD1315_UGUI_H

#include "ugui.h"

int ssd1315_printf(int x, int y, const char *fmt, ...);
void UG_DrawPixel_SSD1315(UG_S16 x, UG_S16 y, UG_COLOR c);

#endif



#ifndef SSD1315__H
#define SSD1315__H

#include <stdint.h>
#include <stdbool.h>

#define SSD1315_W 128
#define SSD1315_H 64
#define SSD1315_FB_SIZE ((SSD1315_W*SSD1315_H)/8) // 1024 bytes @128x64

bool init_ssd1315(void);
void ssd1315_set_contrast(uint8_t x);    // 0..255
void ssd1315_power(bool on);
void ssd1315_clear_back(void);           // clear back buffer
void ssd1315_flush(void);                // push back buffer to panel
void ssd1315_swap(void);                 // swap front/back
void ssd1315_line(int x0, int y0, int x1, int y1, int color);
void ssd1315_putpix(int x, int y, int color); // color: 0/1

/* Get raw back buffer (if you want to custom-fill) */
uint8_t *ssd1315_backbuf(void);

#endif


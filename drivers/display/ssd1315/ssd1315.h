
#ifndef SSD1315__H
#define SSD1315__H

#include <stdint.h>
#include <stdbool.h>

#define SSD1315_W 128
#define SSD1315_H 64
#define SSD1315_FB_SIZE ((SSD1315_W*SSD1315_H)/8) // 1024 bytes @128x64

bool init_ssd1315(uint32_t i2c_periph_address_init);
void ssd1315_set_contrast(uint8_t x);    // 0..255
void ssd1315_power(bool on);
void ssd1315_clear_back(void);           // clear back buffer
void ssd1315_flush(void);                // push back buffer to panel
void ssd1315_swap(void);                 // swap front/back
void ssd1315_line(int x0, int y0, int x1, int y1, int color);
void ssd1315_putpix(int x, int y, int color); // color: 0/1
void ssd1315_vshift(uint8_t pixels);     // vshift pixels (max 63)
void ssd1315_rotate_180(void);

/* fading_mode    0:disabled 2:fade out 3:blinking
   interval       0:8 frames .. 0xf:128 frames */
void ssd1315_blinking_mode(uint8_t fading_mode, uint8_t time_interval);

/* Get raw buffers (if you want to custom-fill) */
uint8_t *ssd1315_frontbuf(void);
uint8_t *ssd1315_backbuf(void);

#endif


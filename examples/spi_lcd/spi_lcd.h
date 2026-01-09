
#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#define WIDTH 320
#define HEIGHT 240

float fast_cosf(float x);
float fast_sinf(const float phase);
int oled_dma_printf(int x, int y, const char *fmt, ...);
void display_pattern(uint32_t pat);
void led_brightness(uint8_t pct);
void led_pwm_init(uint32_t arr);
void sine_lut_fill(void);
void fpu_enable(void);
void init_dma_spi(void);
void init_julia_palette(void);
void init_pins(void);
void lcd_flush_fb(uint16_t *framebuffer);
void lcd_hard_reset(void);
void lcd_init(void);
void log_lut_fill(void);
void render_fire(void);
void render_julia(float t);
void render_plasma(float time);
void render_rotozoom(float time);
void render_tunnel(float time);
void render_voxel(int camX, int camY, int camH, int angle, int horizon);
void spi_init(void);
void fb_put_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

#endif


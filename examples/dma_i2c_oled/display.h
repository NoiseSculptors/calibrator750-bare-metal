
#ifndef HW_DISPLAY_H
#define HW_DISPLAY_H

#include <stdint.h>

#define OLED_WIDTH     128
#define OLED_HEIGHT    64
#define OLED_ADDR 0x3C
#define OLED_ROWS 8
#define HDR_SIZE  3    // only page + data-control
#define STRIDE_SIZE    (HDR_SIZE + OLED_WIDTH)

int oled_dma_printf(int x, int y, const char *fmt, ...);
void noisesculptors_logo(void);
void oled_clear_framebuffer(void);
void oled_init_framebuffer(void);
void oled_send_frame_dma_service(uint32_t periph);
void oled_sync_fb(uint8_t *bbuf);
void ssd1315_init_dma(uint32_t periph);
void ssd1315_row_flush(uint32_t periph, uint8_t *buf, uint8_t row);

#endif


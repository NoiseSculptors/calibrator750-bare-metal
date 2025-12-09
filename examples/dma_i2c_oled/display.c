
#include "delay.h"
#include "display.h"
#include "i2c.h"
#include "noisesculptors_image.h"
#include "printf.h"
#include "rng.h"
#include "ssd1315.h"
#include "ssd1315_UGUI.h"
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

uint8_t oled_buf[OLED_ROWS * STRIDE_SIZE];
extern uint8_t font_8x8_linux[256*8];

/* 8 rows * 14 characters + null byte */
#define PRINTF_BUFSZ (8*14)+1

int oled_dma_printf(int x, int y, const char *fmt, ...)
{
    char buf[PRINTF_BUFSZ];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if(!buf[0]) return n;

    int col = x;
    int row = y;

    for(const char *p = buf; *p; p++){
        if(*p=='\n' || col>=14){
            col=0;
            row++;
            if(row>=OLED_ROWS) break;
            if(*p=='\n') continue;
        }

        uint8_t *dst = oled_buf + 1 + /* 1, for 1 empty line on the left, 
                                         after that, there will be 1 empty
                                         line on the right */
                       row*STRIDE_SIZE +
                       HDR_SIZE +
                       col*9;

        const uint8_t *g = font_8x8_linux + (*p)*8;

        for(int i=0;i<8;i++)
            dst[i] = g[i];

        dst[8] = 0;

        col++;
    }

    return n;
}

void oled_clear_framebuffer(void)
{
    for (uint8_t row = 0; row < OLED_ROWS; row++)
        memset(&oled_buf[(row * STRIDE_SIZE) + HDR_SIZE], 0x00, OLED_WIDTH);
}

static void ssd1315_page_addressing_mode(uint32_t periph)
{
    i2c_write_bytes(periph, OLED_ADDR, (uint8_t[]){
               0x00, 0x20, 0x02, // Set Page Addressing Mode (0x00/0x01/0x02)
               0x00, 0x00,       // lower column = 0
               0x00, 0x10,       // higher column = 0
           }, 7);
}

void ssd1315_init_dma(uint32_t periph)
{
    ssd1315_page_addressing_mode(periph);

    for (uint8_t row = 0; row < OLED_ROWS; row++)
    {
        uint8_t *b = &oled_buf[row * STRIDE_SIZE];

// NOTE:
// The control byte for commands *must* use the continuation bit (0x80)
// if command bytes and the following data bytes are sent in the same
// I2C transaction (same START..STOP frame).

        // Minimal header per row
        b[0] = 0x80;                   // cmd control 
        b[1] = 0xB0 | (row & 0x0F);    // page address
        b[2] = 0x40;                   // DATA

     // Clear pixel bytes
        memset(&b[HDR_SIZE], 0x55, OLED_WIDTH);
    }
}

void oled_sync_fb(uint8_t *buf)
{
    for (uint32_t row = 0; row < OLED_ROWS; row++)
    {
        uint8_t *dst = &oled_buf[(row * STRIDE_SIZE) + HDR_SIZE];
        uint8_t *src = &buf[row * OLED_WIDTH];

        for (uint32_t i = 0; i < OLED_WIDTH; i++)
            dst[i] = src[i];
    }
}

static inline void oled_set_pixel(int x, int y, uint8_t color)
{
    if ((unsigned)x >= OLED_WIDTH || (unsigned)y >= OLED_HEIGHT) return;

    uint8_t page = (uint8_t)(y >> 3);
    uint8_t bit  = (uint8_t)(y & 7);

    uint8_t *row = &oled_buf[page * STRIDE_SIZE + HDR_SIZE];
    if(color==0)
        row[x] &= (uint8_t)~(1u << bit);
    else
        row[x] |= (uint8_t)(1u << bit);
}

void oled_set_pixel_ugui(UG_S16 x, UG_S16 y, UG_COLOR c)
{
    oled_set_pixel((int)x,(int)y,(uint8_t)c);
}

static inline void oled_clear_pixels(void)
{
    for (uint32_t page = 0; page < OLED_ROWS; page++)
    {
        uint8_t *p = &oled_buf[page * STRIDE_SIZE + HDR_SIZE];
        memset(p, 0x00, OLED_WIDTH);
    }
}

void ssd1315_row_flush(uint32_t periph, uint8_t *buf, uint8_t row)
{
    const uint8_t header_sz = 7;
    const uint8_t row_sz = 128;
    uint8_t bytes[row_sz + header_sz];

    bytes[0] = 0x80;
    bytes[1] = 0x00;
    bytes[2] = 0x80;
    bytes[3] = 0x10;
    bytes[4] = 0x80;
    bytes[5] = 0xb0|(row&0xf);
    bytes[6] = 0x40;

    for(int i=0;i<128; i++)
        bytes[i+header_sz] = buf[i];

    i2c_write_bytes(periph, OLED_ADDR, bytes, row_sz + header_sz); 
}

void noisesculptors_logo(void)
{
    uint8_t tmp[1024]; 

    /* fill_buffer_with_noise */
    for(int i=0;i<1024;i++)
        tmp[i] = rng_rnd();

    /* display logo */
    for(int i=0;i<768;i++)
        tmp[i+128] |= img[i];

    oled_sync_fb(tmp);
}


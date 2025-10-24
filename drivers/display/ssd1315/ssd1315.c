
#include "delay.h"
#include "gpio.h"
#include "i2c.h"
#include "memory.h"
#include "rcc.h"
#include "ssd1315.h"
#include <stdbool.h>
#include <stdint.h>

#define I2C_OLED_ADDR 0x3C   /* common 128x64 modules */

static uint32_t i2c_hw_addr;

static inline void tiny_delay(unsigned n)
{
    for (volatile unsigned i=0; i<n; i++) __asm__ volatile("nop");
}

/* ===== Double buffers (front shown, back drawn) ===== */
static uint8_t fb0[SSD1315_FB_SIZE];
static uint8_t fb1[SSD1315_FB_SIZE];

static uint8_t *front = fb0;
static uint8_t *back  = fb1;

/* ---- COMMAND (control byte 0x00) ---- */
static inline void ssd1315_cmd(uint8_t c)
{
    uint8_t buf[2] = { 0x00, c }; /* 0x00 = command stream */
    i2c_write_bytes(i2c_hw_addr, I2C_OLED_ADDR, buf, 2);
}

/* ---- DATA (control byte 0x40) ---- */
static inline void ssd1315_data_bytes(const uint8_t *p, uint16_t n)
{

/* 32B is safe, but a bit slower, 254 largest possible */
#define CHUNK 254
    while (n) {
        uint16_t chunk = (n > CHUNK) ? CHUNK: n;
        uint8_t buf[1+CHUNK];
        buf[0] = 0x40;
        for (uint16_t i=0; i<chunk; i++) buf[1+i] = p[i];
        i2c_write_bytes(i2c_hw_addr, I2C_OLED_ADDR, buf, (uint16_t)(1+chunk));
        p += chunk;
        n -= chunk;
    }

}

static void ssd1315_set_addr_window(uint8_t x0, uint8_t x1, uint8_t page0, uint8_t page1)
{
    ssd1315_cmd(0x21); ssd1315_cmd(x0); ssd1315_cmd(x1);       /* Column addr */
    ssd1315_cmd(0x22); ssd1315_cmd(page0); ssd1315_cmd(page1); /* Page addr   */
}

bool init_ssd1315(uint32_t i2c_hw_addr_init)
{
    i2c_hw_addr = i2c_hw_addr_init;

    delay_ms(200);

    /* slightly modified init sequence from ssd1315 datasheet */
    ssd1315_cmd(0xAE);//turn off oled panel
    ssd1315_cmd(0x00);//set low column address
    ssd1315_cmd(0x10);//set high column address
    ssd1315_cmd(0x40);//set start line address
    ssd1315_cmd(0x81);//set contrast control register
    ssd1315_cmd(0xCF);//Set SEG Output Current Brightness
    ssd1315_cmd(0xA1);//Set SEG/Column Mapping 0xa0
    ssd1315_cmd(0xC8);//Set COM/Row Scan Direction 0xc0
    ssd1315_cmd(0xA6);//set normal display
    ssd1315_cmd(0xA8);//set multiplex ratio(1 to 64)
    ssd1315_cmd(0x3f);//1/64 duty
    ssd1315_cmd(0xD3);//set display offset Shift Mapping RAM Counter(0x00~0x3F)
    ssd1315_cmd(0x00);//not offset
    ssd1315_cmd(0xd5);//set display clock divide ratio/oscillator frequency
    ssd1315_cmd(0x80);//set divide ratio, Set Clock as 100 Frames/Sec
    ssd1315_cmd(0xD9);//set precharge period
    ssd1315_cmd(0xF1);//Set PreCharge as 15 Clocks & Discharge as 1 Clock
    ssd1315_cmd(0xDA);//set com pins hardware configuration
    ssd1315_cmd(0x12);
    ssd1315_cmd(0xDB);//set vcomh
    ssd1315_cmd(0x30);//Set VCOM Deselect Level
    ssd1315_cmd(0x20);//Set Page Addressing Mode (0x00/0x01/0x02)
    ssd1315_cmd(0x00);//
    ssd1315_cmd(0x8D);//set Charge Pump enable/disable
    ssd1315_cmd(0x14);//set(0x10) disable
    ssd1315_clear_back();
    ssd1315_cmd(0xAF);

    delay_ms(100);

    ssd1315_flush();
    return true;
}

/* max 63 */
void ssd1315_vshift(uint8_t pixels)
{
    ssd1315_cmd(0x40 | (pixels & 0x3F));
}

void ssd1315_rotate_180(void)
{
    ssd1315_cmd(0xA0); // SEG remap (flip horizontally)
    ssd1315_cmd(0xC0); // COM scan direction (flip vertically)
}

void ssd1315_set_contrast(uint8_t x)
{
    ssd1315_cmd(0x81); ssd1315_cmd(x);
}

void ssd1315_power(bool on)
{
    ssd1315_cmd(on ? 0xAF : 0xAE);
}

void ssd1315_clear_back(void)
{
    for (int i=0; i<SSD1315_FB_SIZE; i++) back[i] = 0x00;
}

void ssd1315_flush(void)
{
    /* Push full buffer: 8 pages × 128 columns for 128×64 */
    ssd1315_set_addr_window(0, SSD1315_W-1, 0, (SSD1315_H/8)-1);
    ssd1315_data_bytes(back, SSD1315_FB_SIZE);
}

void ssd1315_swap(void)
{
    uint8_t *tmp = front; front = back; back = tmp;
}

/* 1bpp pixel write into back buffer */
void ssd1315_putpix(int x, int y, int color)
{
    if ((unsigned)x >= SSD1315_W || (unsigned)y >= SSD1315_H) return;

    uint32_t idx = (uint32_t)(y >> 3) * (uint32_t)SSD1315_W + (uint32_t)x;
    uint8_t  msk = (uint8_t)(1u << (y & 7));

    if (color) back[idx] |=  msk;
    else       back[idx] &= ~msk;
}

/* Bresenham line on 1bpp */
void ssd1315_line(int x0, int y0, int x1, int y1, int color)
{
    int dx = (x1>x0)? (x1-x0) : (x0-x1);
    int sx = (x0<x1)? 1 : -1;
    int dy = (y1>y0)? (y0-y1) : (y1-y0);
    int sy = (y0<y1)? 1 : -1;
    int err = dx + dy;
    for (;;) {
        ssd1315_putpix(x0,y0,color);
        if (x0==x1 && y0==y1) break;
        int e2 = 2*err;
        if (e2 >= dy){ err += dy; x0 += sx; }
        if (e2 <= dx){ err += dx; y0 += sy; }
    }
}

void ssd1315_blinking_mode(uint8_t fading_mode, uint8_t time_interval){
    uint8_t setting = 0;
    ssd1315_cmd(0x23);
    setting |= ((fading_mode & 0x3)<<4)  |
               ((time_interval & 0xf)<<0);
    ssd1315_cmd(setting);
}

uint8_t *ssd1315_frontbuf(void) { return front; }
uint8_t *ssd1315_backbuf(void) { return back; }


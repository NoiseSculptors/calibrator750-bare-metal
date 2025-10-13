
#include "delay.h"
#include "gpio.h"
#include "i2c.h"
#include "memory.h"
#include "rcc.h"
#include "ugui.h"
#include "ssd1315.h"
#include <stdbool.h>
#include <stdint.h>

/* --- Pick the I2C3 register aliases --- */
#define I2C_ISR     I2C3_ISR
#define I2C_CR1     I2C3_CR1
#define I2C_CR2     I2C3_CR2     /* FIX: was I2C3_CR1 by mistake */
#define I2C_TXDR    I2C3_TXDR
#define I2C_TIMINGR I2C3_TIMINGR
#define I2C_ICR     I2C3_ICR

/* ===== SSD1315 high-level ===== */
#define I2C_ADDR 0x3C   /* common 128x64 modules */

#define SSD1315_PRINTF_BUFSZ 256

static inline void tiny_delay(unsigned n)
{
    for (volatile unsigned i=0; i<n; i++) __asm__ volatile("nop");
}

/* ===== Double buffers (front shown, back drawn) ===== */
static uint8_t fb0[SSD1315_FB_SIZE];
static uint8_t fb1[SSD1315_FB_SIZE];
static uint8_t *front = fb0;
static uint8_t *back  = fb1;

/* --- PINS (I2C3 on PA8=SCL, PC9=SDA) --- */
static void i2c3_init_pins_bus(void)
{
    *RCC_AHB4ENR  |= (1u << GPIOAEN) | (1u << GPIOCEN);

#define I2C3EN 23
    *RCC_APB1LENR |= (1u << I2C3EN);    /* enable I2C3 peripheral clock */

    /* PA8 → AF4, open-drain, pull-up */
    *GPIOA_MODER   &= ~(0x3u << (MODER8));
    *GPIOA_MODER   |=  (0x2u << (MODER8));  /* AF */
    *GPIOA_OTYPER  |=  (0x1u << (OT8));     /* open-drain */
    *GPIOA_PUPDR   &= ~(0x3u << (PUPDR8));
    *GPIOA_PUPDR   |=  (0x1u << (PUPDR8));  /* pull-up */
    *GPIOA_AFRH    &= ~(0xFu << (AFR8));
    *GPIOA_AFRH    |=  (0x4u << (AFR8));    /* AF4 I2C3_SCL */
    /* PC9 → AF4, open-drain, pull-up */
    *GPIOC_MODER   &= ~(0x3u << (MODER9));
    *GPIOC_MODER   |=  (0x2u << (MODER9));  /* AF */
    *GPIOC_OTYPER  |=  (0x1u << (OT9));     /* open-drain */
    *GPIOC_PUPDR   &= ~(0x3u << (PUPDR9));
    *GPIOC_PUPDR   |=  (0x1u << (PUPDR9));  /* pull-up */
    *GPIOC_AFRH    &= ~(0xFu << (AFR9));
    *GPIOC_AFRH    |=  (0x4u << (AFR9));    /* AF4 I2C3_SDA */

    /* Disable, program TIMINGR, enable */
    *I2C_CR1 = 0;
    
    /* pick the one that works best for you */
//  *I2C_TIMINGR = 0x00C0216C; /* ca 626kHz */
//  *I2C_TIMINGR = 0x00C0224C; /* ca 749kHz */
//  *I2C_TIMINGR = 0x00C01B3D; /* ca 930kHz */
//  *I2C_TIMINGR = 0x00C00A19; /* ca 1.58MHz */
//  *I2C_TIMINGR = 0x00C0050C; /* ca 2MHz */
//  *I2C_TIMINGR = 0x00C00407; /* ca 3MHz */
    *I2C_TIMINGR = 0x00700404; /* ca 3.33MHz max stable I could achieve */

#define ANFOFF 12 /* analog filter 0:on 1:off, default:0, for reaching
                     higher frequencies */
#define PE 0
    *I2C_CR1 = (1<<ANFOFF)|(1<<PE);                          /* PE=1 */
}

static void i2c3_start_write(uint8_t addr, uint16_t nbytes)
{
    /* CR2: SADD(7-bit<<1), RD_WRN=0, NBYTES, START=1, AUTOEND=0 */
    unsigned cr2 = ((unsigned)(addr) << 1) | ((unsigned)nbytes << 16) | (1u << 13);
    *I2C_CR2 = cr2;
}

static void i2c3_wait_txis(void) { while(((*I2C_ISR) & (1u<<1)) == 0){} } /* TXIS */
static void i2c3_wait_tc(void)   { while(((*I2C_ISR) & (1u<<6)) == 0){} } /* TC   */
static void i2c3_stop(void)      { *I2C_CR2 |= (1u<<14); }                /* STOP */

/* Write N bytes in a single write transfer (blocking) */
static void i2c3_write_bytes(uint8_t addr, const uint8_t *data, uint16_t n)
{
    if (n == 0) return;
    i2c3_start_write(addr, n);
    for (uint16_t i=0; i<n; i++) {
        i2c3_wait_txis();
        *I2C_TXDR = data[i];
    }
    i2c3_wait_tc();
    i2c3_stop();
}

#if 0
#define AUTOEND 25
#define START 13
#define TXIS 1
#define STOPF 5
#define STOPCF 5
static void i2c3_write_bytes(uint8_t address, const uint8_t *data, uint16_t data_length)
{
    *I2C_CR2 =
          (address << 1)
        | (data_length << 16)
        | (1 << AUTOEND)  /* auto end */
        | (1 << START); /* start generation */

    for(int i=0; i<data_length; i++){
        while(!(*I2C_ISR & (1<<TXIS)));
        *I2C_TXDR = data[i];

    }

    /* wait for STOPF or BUSY */
    while(!(*I2C_ISR & (1 << STOPF)));
    *I2C_ICR |= (1 << STOPCF);
}
#endif

/* ---- COMMAND (control byte 0x00) ---- */
static inline void ssd1315_cmd(uint8_t c)
{
    uint8_t buf[2] = { 0x00, c }; /* 0x00 = command stream */
    i2c3_write_bytes(I2C_ADDR, buf, 2);
} /* <<< FIX: close the function properly */

/* ---- DATA (control byte 0x40) ---- */
static inline void ssd1315_data_bytes(const uint8_t *p, uint16_t n)
{
    /* Send in small chunks: [0x40, payload...] */
    while (n) {
        uint16_t chunk = (n > 32) ? 32 : n; /* 32B is safe; 16B also fine */
        uint8_t buf[1+32];
        buf[0] = 0x40;
        for (uint16_t i=0; i<chunk; i++) buf[1+i] = p[i];
        i2c3_write_bytes(I2C_ADDR, buf, (uint16_t)(1+chunk));
        p += chunk;
        n -= chunk;
    }
}

static void ssd1315_set_addr_window(uint8_t x0, uint8_t x1, uint8_t page0, uint8_t page1)
{
    ssd1315_cmd(0x21); ssd1315_cmd(x0); ssd1315_cmd(x1);       /* Column addr */
    ssd1315_cmd(0x22); ssd1315_cmd(page0); ssd1315_cmd(page1); /* Page addr   */
}

bool init_ssd1315(void)
{
    i2c3_init_pins_bus();

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

void ssd1315_set_contrast(uint8_t x) { ssd1315_cmd(0x81); ssd1315_cmd(x); }
void ssd1315_power(bool on)          { ssd1315_cmd(on ? 0xAF : 0xAE); }

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

uint8_t *ssd1315_backbuf(void) { return back; }

void UG_DrawPixel_SSD1315(UG_S16 x, UG_S16 y, UG_COLOR c)
{
    // uGUI uses 16-bit RGB565 "color"; treat nonzero as ON
    ssd1315_putpix((int)x, (int)y, c==0?C_BLACK:C_WHITE);
}


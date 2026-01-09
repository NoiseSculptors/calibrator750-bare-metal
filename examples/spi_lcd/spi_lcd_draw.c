
#include "delay.h"
#include "spi_lcd.h"
#include "rng.h"
#include <stdint.h>

extern uint16_t fb[WIDTH*HEIGHT] __attribute__((aligned(32))); 
static uint16_t palette[256];

/* pattern IDs */
enum { PAT_BARS=0,
       PAT_HGRAD,
       PAT_CHECKER,
       PAT_STRIPES,
       PAT_RADIAL,
       PAT_RG_GRAD };

/* rgb888 -> rgb565 */
static inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t px=(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
    return __builtin_bswap16(px);
}

/* fill fb[] with chosen pattern. pixels stored MSB then LSB (big-endian) */
static void fill_test_pattern(int pat)
{
    uint32_t idx = 0;
    const uint32_t maxd2 = (WIDTH/2)*(WIDTH/2) + (HEIGHT/2)*(HEIGHT/2);
    static const uint16_t bars[8] = { 0xF800, /* R */
                                      0x07E0, /* G */
                                      0x001F, /* B */
                                      0xFFE0, /* Y */
                                      0x07FF, /* C */
                                      0xF81F, /* M */
                                      0xFFFF, /* W */
                                      0x0000  /* K */ };

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            uint16_t px;
            switch (pat) {
            case PAT_BARS: {
                unsigned bar = (x * 8) / WIDTH;
                px = bars[bar];
            } break;

            case PAT_HGRAD: {
                uint8_t v = (x * 255) / (WIDTH - 1);
                px = rgb565(v, v, v);
            } break;

            case PAT_CHECKER: {
                px = ((((x >> 4) + (y >> 4)) & 1) ? 0xFFFF : 0x0000);
            } break;

            case PAT_STRIPES: {
                unsigned s = (y * 3) / HEIGHT;
                px = (s == 0) ? 0xF800 : ((s == 1) ? 0x07E0 : 0x001F);
            } break;

            case PAT_RADIAL: {
                int dx = x - (WIDTH / 2);
                int dy = y - (HEIGHT / 2);
                uint32_t d2 = (uint32_t)(dx*dx + dy*dy);
                uint8_t v = (d2 >= maxd2) ? 0 : (uint8_t)(255 - (d2 * 255 / maxd2));
                px = rgb565(v, v/2, 255 - v);
            } break;

            case PAT_RG_GRAD: {
                uint8_t r = (x * 255) / (WIDTH - 1);
                uint8_t g = (y * 255) / (HEIGHT - 1);
                px = rgb565(r, g, 128);
            } break;
          }
            fb[idx++] = px;
        }
    }
}

void display_pattern(uint32_t pat)
{
    fill_test_pattern(pat);
}

#define NUM 48

static inline void clear_fb(void) {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        fb[i] = 0;
}

static inline void draw_square(
    int cx, int cy, int r,
    uint16_t color
) {
    int x0 = cx - r;
    int x1 = cx + r;
    int y0 = cy - r;
    int y1 = cy + r;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= WIDTH) x1 = WIDTH - 1;
    if (y1 >= HEIGHT) y1 = HEIGHT - 1;

    int row0 = y0 * WIDTH;
    int row1 = y1 * WIDTH;

    for (int x = x0; x <= x1; x++) {
        fb[row0 + x] = color;
        fb[row1 + x] = color;
    }

    for (int y = y0; y <= y1; y++) {
        int row = y * WIDTH;
        fb[row + x0] = color;
        fb[row + x1] = color;
    }
}

void render_tunnel(float time) {
    clear_fb();

    int cx0 = WIDTH / 2;
    int cy0 = HEIGHT / 2;

    for (int i = 0; i < NUM; i++) {
        float z = (float)i / NUM;

        float sx = fast_sinf(time + z * 6.0f);
        float sy = fast_sinf(time * 0.7f + z * 5.0f);

        int cx = cx0 + (int)(sx * 30.0f);
        int cy = cy0 + (int)(sy * 20.0f);

        int r = (int)((1.0f - z) * 140.0f);
        if (r <= 1) continue;

        uint16_t color =
            ((31 - i) << 11) |  // red
            ((i & 63) << 5)  |  // green
            (i & 31);           // blue

        color = __builtin_bswap16(color);

        draw_square(cx, cy, r, color);
    }
}

static void init_palette(void) {
    for (int i = 0; i < 256; i++) {
        int r = (fast_sinf(i * 0.024f) * 0.5f + 0.5f) * 31;
        int g = (fast_sinf(i * 0.031f + 2.0f) * 0.5f + 0.5f) * 63;
        int b = (fast_sinf(i * 0.017f + 4.0f) * 0.5f + 0.5f) * 31;

        palette[i] = (r << 11) | (g << 5) | b;
    }
}

void render_plasma(float time) {
    static int init = 0;
    if (!init) {
        init_palette();
        init = 1;
    }

    int idx = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            float v =
                fast_sinf(x * 0.062f + time) +
                fast_sinf(y * 0.048f + time * 1.3f) +
                fast_sinf((x + y) * 0.044f + time * 0.7f);

            int c = (int)((v + 3.0f) * 42.0f);  // map to 0..255
            if (c < 0) c = 0;
            if (c > 255) c = 255;

            fb[idx++] = __builtin_bswap16(palette[c]);
        }
    }
}


#define TEX_SIZE 64
#define TEX_MASK (TEX_SIZE - 1)

uint16_t texture[TEX_SIZE * TEX_SIZE];

static void init_texture(void)
{
    uint32_t i=0;
    for (int y = 0; y < TEX_SIZE; ++y)
        for (int x = 0; x < TEX_SIZE; ++x)
           texture[i++] = ((((x >> 4) + (y >> 4)) & 1) ? 0xFFFF : 0x0000);
}

void render_rotozoom(float time) {

    static int init = 0;
    if (!init) {
        init_texture();
        init = 1;
    }

    float angle = time * 0.1f;
    float zoom  = fast_sinf(time * 0.5f) * 0.5f + 1.2f;

    float ca = fast_cosf(angle) / zoom;
    float sa = fast_sinf(angle) / zoom;

    int cx = WIDTH  / 2;
    int cy = HEIGHT / 2;

    int idx = 0;

    for (int y = 0; y < HEIGHT; y++) {
        float fy = (float)(y - cy);

        float u = -ca * cx + sa * fy;
        float v = -sa * cx - ca * fy;

        for (int x = 0; x < WIDTH; x++) {
            int tu = ((int)u) & TEX_MASK;
            int tv = ((int)v) & TEX_MASK;

            fb[idx++] = texture[(tv << 6) + tu];

            u += ca;
            v += sa;
        }
    }
}


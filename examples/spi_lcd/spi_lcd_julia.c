
#include "spi_lcd.h"
#include <math.h>
#include <stdint.h>

#define PI 3.141592653589793
#define TWO_PI 6.283185307179586

#define SINE_LUT_SIZE 8192u 
#define SINE_LUT_MASK (SINE_LUT_SIZE - 1u) // must be power of two

#define LUT_SIZE 8192
#define LN2 0.6931471805599453f

// 320x240(16bit RGB 5-6-5)
extern uint16_t fb[WIDTH*HEIGHT] __attribute__((aligned(32))); 

// Precomputed scale to convert radians -> LUT index
static const float k_rad_to_idx = (float)SINE_LUT_SIZE / TWO_PI;

__attribute__((aligned(32)))
static float g_sine_lut[SINE_LUT_SIZE];

void sine_lut_fill(void)
{
    const float step = TWO_PI / (float)SINE_LUT_SIZE;
    const float offset = 0.5f * step; // midpoint
    for (uint32_t i = 0; i < SINE_LUT_SIZE; ++i) {
        g_sine_lut[i] = sinf(offset + step * (float)i);
    }
}

// No-interp lookup (nearest/lower bin). phase ∈ [0, 2π]
float fast_sinf(const float phase)
{
    // Assumes 'phase' already wrapped to [0, 2π); avoid fmodf here.
    uint32_t idx = (uint32_t)(phase * k_rad_to_idx);
    idx &= SINE_LUT_MASK; // cheap modulo (size must be power of two)
    return g_sine_lut[idx];
}

float fast_cosf(float x)
{
    return fast_sinf(x + (PI * 0.5f));
}

static uint16_t julia_palette[32768];

void init_julia_palette(void)
{
    for (int i = 0; i < 32768; i++) {
        float t = i / 32767.0f;

        float r = 0.5f + 0.5f * cosf(6.28318f * (t + 0.00f));
        float g = 0.5f + 0.5f * cosf(6.28318f * (t + 0.33f));
        float b = 0.5f + 0.5f * cosf(6.28318f * (t + 0.33f));

        uint8_t rr = (uint8_t)(r * 255.0f);
        uint8_t gg = (uint8_t)(g * 255.0f);
        uint8_t bb = (uint8_t)(b * 255.0f);

        uint16_t px =
            ((bb & 0xF8) << 8) |
            ((gg & 0xFC) << 3) |
            ( rr >> 3);

        julia_palette[i] = __builtin_bswap16(px);
    }
}

void render_julia(float t)
{
    const float halfw = WIDTH * 0.5f;
    const float halfh = HEIGHT * 0.5f;

    float ang = t * 0.40f;
    float cr  = 0.7885f * fast_cosf(ang);
    float ci  = 0.7885f * fast_sinf(ang);

    float zoom_div = t *0.9f- 3.3f;

    float cx0 = -0.1f + 0.15f * fast_sinf(0.11f * t);
    float cy0 =  0.0f + 0.10f * fast_cosf(0.07f * t);

    const int   maxit = 60;
    const float inv_maxit = 1.0f * 0.04; // 1/20

    float sx = (2.8f * zoom_div) * 0.003125f; // 1/width
    float sy = (1.8f * zoom_div) * 0.0041666667f; // 1/height

    uint16_t *p = fb;

    for (int y = 0; y < HEIGHT; y++) {

        float zy0 = ((float)y - halfh) * sy + cy0;
        float zx0 = (-halfw * sx) + cx0;

        float zx_start = zx0;

        for (int x = 0; x < WIDTH; x++) {

            float zx = zx_start;
            float zy = zy0;
            zx_start += sx;

            int it = 0;
            float zx2, zy2;

            while (it < maxit) {
                zx2 = zx * zx;
                zy2 = zy * zy;

                if (zx2 + zy2 > 3.5f)   // smaller radius = earlier escape
                    break;

                float zxy = zx * zy;
                zx = zx2 - zy2 + cr;
                zy = zxy + zxy + ci;

                it++;
            }

            if (it == maxit) {
                *p++ = 0x0000;
                continue;
            }

            float mag2 = zx2 + zy2;
            float mu = it - mag2 * 0.1f;


            int idx = (int)(mu * (32768.0f * inv_maxit));
            if (idx < 0) idx = 0;
            if (idx > 32768) idx = 32768;

            *p++ = julia_palette[idx];
        }
    }
}


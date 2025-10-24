
#include "i2c.h"
#include "init.h"
#include "memory.h"
#include "ssd1315.h"
#include "serial.h"
#include "printf.h"
#include <math.h>
#include <stdint.h>

// ------------ Display config ------------
#define OLED_W 128
#define OLED_H  64

// ----------- FPU / DWT helpers ----------
static inline void fpu_enable(void){
    // CP10/CP11 full access
    *(volatile uint32_t*)0xE000ED88 |= (0xFu << 20);
    __asm volatile("dsb; isb");
}

// ----------- 4x4 Bayer dither -----------
static const uint8_t bayer4[4][4] = {
    {  0,  8,  2, 10},
    { 12,  4, 14,  6},
    {  3, 11,  1,  9},
    { 15,  7, 13,  5}
};

// Map a 0..1 “shade” to on/off using Bayer
static inline int dither_on(float shade, int x, int y){
    // clamp
    if (shade < 0.f) shade = 0.f; else if (shade > 1.f) shade = 1.f;
    float t = shade * 16.f;                    // 0..16
    return (float)bayer4[y & 3][x & 3] < t;    // thresholded ordered dither
}

// ----------- Julia renderer -------------
// Fill fb with a Julia set at time t (seconds).
static void render_julia(float t){

    // Animate parameter c on the unit circle
    float ang = t * 0.35f;
    float cr = 0.7885f * cosf(ang);
    float ci = 0.7885f * sinf(ang);

    // View window (animated zoom/pan a touch)
    float zoom = 1.6f + 0.3f*sinf(0.27f*t);     // ~[-1.8,1.8] to [-1.4,1.4]
    float cx0  = -0.1f + 0.15f*sinf(0.11f*t);
    float cy0  =  0.0f + 0.10f*cosf(0.07f*t);

    // Iteration budget (cheap knob for quality/perf)
    const int maxit = 48; // H750 can handle this easily per frame

    // Precompute pixel-to-complex scale
    float sx = (2.8f/zoom) / (float)OLED_W;
    float sy = (1.8f/zoom) / (float)OLED_H;

    for (int y = 0; y < OLED_H; ++y){
        float zy0 = ( (float)y - (OLED_H*0.5f) ) * sy + cy0;
        for (int x = 0; x < OLED_W; ++x){
            float zx = ( (float)x - (OLED_W*0.5f) ) * sx + cx0;
            float zy = zy0;

            int it = 0;
            float zx2 = 0.f, zy2 = 0.f;
            // z = z^2 + c, bailout radius 2 (|z|^2 > 4)
            while (it < maxit){
                zx2 = zx*zx; zy2 = zy*zy;
                if ((zx2 + zy2) > 4.0f) break;
                // (zx + i*zy)^2 = (zx^2 - zy^2) + i*(2*zx*zy)
                float nx = zx2 - zy2 + cr;
                zy = (2.0f*zx*zy) + ci;
                zx = nx;
                ++it;
            }

            // Smooth shade (normalized iteration count)
            float shade;
            if (it >= maxit){
                shade = 0.f; // inside -> black
            } else {
                // Continuous (log) coloring for nicer gradients
                float mag = sqrtf(zx2 + zy2);
                float mu = (float)it + 1.f - logf(logf(mag))/logf(2.0f);
                shade = fminf(1.f, mu / (float)maxit); // 0..1
            }

            if (dither_on(shade, x, y)) ssd1315_putpix(x, y, 1);
        }
    }
}

int main(void){
    fpu_enable();
    clock_info_t ci;
    ci = init_clock();
    init_i2c3_pa8_pc9(&ci);
    init_ssd1315(I2C3);
    ssd1315_set_contrast(1);

    float x=0.0f;

    for (;;){
        ssd1315_clear_back();
        render_julia(x);
        ssd1315_flush();
        ssd1315_swap();
        x+=0.005f;
    }
}


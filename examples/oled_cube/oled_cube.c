
#include "delay.h"
#include "init.h"
#include "ssd1315.h"
#include "usart1.h"
#include <stdbool.h>
#include <stdint.h>

// --- Config ---
#define PI     3.1415926f
#define TWO_PI 6.2831853f

#define SZ 16

float fast_sinf(float x);
float fast_cosf(float x);

static inline float wrap_angle(float x) {
    while (x >  PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;
    return x;
}

typedef struct { float x,y,z; } v3;

static const v3 CUBE[8] = {
    {-SZ,-SZ,-SZ}, {+SZ,-SZ,-SZ}, {+SZ,+SZ,-SZ}, {-SZ,+SZ,-SZ},
    {-SZ,-SZ,+SZ}, {+SZ,-SZ,+SZ}, {+SZ,+SZ,+SZ}, {-SZ,+SZ,+SZ}
};
static const uint8_t E[12][2] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};

static inline v3 rot(v3 p, float ax, float ay){
    float cx = fast_cosf(ax), sx = fast_sinf(ax);
    float cy = fast_cosf(ay), sy = fast_sinf(ay);
    float y = p.y*cx - p.z*sx;
    float z = p.y*sx + p.z*cx;
    float x = p.x;
    float X =  x*cy + z*sy;
    float Z = -x*sy + z*cy;
    return (v3){X, y, Z};
}

static inline bool proj(v3 p, int *sx, int *sy){
    // Fit nicely onto 128x64
    float z = p.z + 36.0f;
    if (z <= 1.0f) return false;
    float f = 24.0f / z;                 // adjust for OLED
    int x = (int)(SSD1315_W/2 + p.x * f * 1.5f);
    int y = (int)(SSD1315_H/2 - p.y * f);
    *sx = x; *sy = y;
    return (x>=-1 && x<=SSD1315_W && y>=-1 && y<=SSD1315_H);
}

static inline void cls(void){
    ssd1315_clear_back();
}

static void draw_frame(float ax, float ay){
    cls();
    v3 v[8];
    for (int i=0;i<8;i++) v[i] = rot(CUBE[i], ax, ay);
    for (int i=0;i<12;i++){
        int a = E[i][0], b = E[i][1];
        int x0,y0,x1,y1;
        if (proj(v[a], &x0,&y0) && proj(v[b], &x1,&y1)){
            ssd1315_line(x0,y0,x1,y1,1);
        }
    }
    ssd1315_flush();
}

int main(void){
    // clocks, minimal delays etc. assumed done in your init code if needed
    init_usart1(init_clock());
    init_ssd1315();
    ssd1315_set_contrast(1);

    float ax = 0.0f, ay = 0.0f;
    for (;;){
        draw_frame(ax, ay);
        ax += 0.0031f;
        ay += 0.0025f;
        // Tiny frame pacing
        for (volatile uint32_t _d=0; _d<40; ++_d) __asm__ volatile("nop");
    }
    return 0;
}

float fast_sinf(float x) {
    x = wrap_angle(x);
    float y = 1.27323954f * x - 0.405284735f * x * ((x < 0) ? -x : x);
    y = 0.775f * y + 0.225f * y * ((y < 0) ? -y : y);
    return y;
}
float fast_cosf(float x) { return fast_sinf(x + 1.5707963f); }


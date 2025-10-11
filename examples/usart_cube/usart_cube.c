
#include "init.h"
#include "printf.h"
#include "usart1.h"
#include <stdint.h>
#include <stdbool.h>

/* displays rotating cube over USART1 PA9/USART1_TX PA10/USART1_RX */

#define W 42
#define H 30
#define SZ 12
#define FRAME_DELAY() for (volatile uint32_t _d=0; _d<1000; ++_d) __asm__ volatile("nop")  // crude delay
#define PI     3.1415926f
#define TWO_PI 6.2831853f

float fast_sinf(float x);
float fast_cosf(float x);

static inline float wrap_angle(float x) {
    // Wrap to [-PI, PI]
    while (x >  PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;
    return x;
}

// --- Minimal math (use float for simplicity; swap to fixed if no FPU) ---
typedef struct { float x,y,z; } v3;

// 8 cube vertices
static const v3 CUBE[8] = {
    {-SZ,-SZ,-SZ}, {+SZ,-SZ,-SZ}, {+SZ,+SZ,-SZ}, {-SZ,+SZ,-SZ},  // back
    {-SZ,-SZ,+SZ}, {+SZ,-SZ,+SZ}, {+SZ,+SZ,+SZ}, {-SZ,+SZ,+SZ}   // front
};

// 12 edges (pairs of vertex indices)
static const uint8_t E[12][2] = {
    {0,1},{1,2},{2,3},{3,0}, // back square
    {4,5},{5,6},{6,7},{7,4}, // front square
    {0,4},{1,5},{2,6},{3,7}  // connections
};

static char fb[W*H];

static inline void cls(void){
    for (int i=0;i<W*H;i++) fb[i]=' ';
}

// Put a char in bounds
static inline void pset(int x,int y,char c){
    if ((unsigned)x<W && (unsigned)y<H) fb[y*W + x] = c;
}

// Bresenham line
static void line(int x0,int y0,int x1,int y1,char c){
    int dx = (x1>x0)? (x1-x0) : (x0-x1);
    int sx = (x0<x1)? 1 : -1;
    int dy = (y1>y0)? (y0-y1) : (y1-y0);
    int sy = (y0<y1)? 1 : -1;
    int err = dx + dy;
    for(;;){
        pset(x0,y0,c);
        if (x0==x1 && y0==y1) break;
        int e2 = 2*err;
        if (e2 >= dy){ err += dy; x0 += sx; }
        if (e2 <= dx){ err += dx; y0 += sy; }
    }
}

// Approximation: sine ≈ 1.27323954*x - 0.405284735*x*|x|
float fast_sinf(float x) {
    x = wrap_angle(x);
    float y = 1.27323954f * x - 0.405284735f * x * ((x < 0) ? -x : x);
    // Optional correction for smoother shape
    y = 0.775f * y + 0.225f * y * ((y < 0) ? -y : y);
    return y;
}

// cosine(x) = sine(x + π/2)
float fast_cosf(float x) {
    return fast_sinf(x + 1.5707963f);
}

// Rotate around X then Y
static inline v3 rot(v3 p, float ax, float ay){
    // Rx
    float cx = fast_cosf(ax), sx = fast_sinf(ax);
    float cy = fast_cosf(ay), sy = fast_sinf(ay);
    float y = p.y*cx - p.z*sx;
    float z = p.y*sx + p.z*cx;
    float x = p.x;
    // Ry
    float X =  x*cy + z*sy;
    float Z = -x*sy + z*cy;
    return (v3){X, y, Z};
}

// Project to 2D screen coords
static inline bool proj(v3 p, int *sx, int *sy){
    // Shift forward to keep Z positive; tune these for size/center
    float z = p.z + 36.0f;
    if (z <= 1.0f) return false;
    float f = 20.0f / z;                 // "focal length"
    int x = (int)(W/2 + p.x * f * 1.5);
    int y = (int)(H/2 - p.y * f);
    *sx = x; *sy = y;
    return (x>=-1 && x<=W && y>=-1 && y<=H);
}

static void draw_frame(float ax, float ay){
    cls();

    // Rotate all vertices
    v3 v[8];
    for (int i=0;i<8;i++) v[i] = rot(CUBE[i], ax, ay);

    // Draw edges
    for (int i=0;i<12;i++){
        int a = E[i][0], b = E[i][1];
        int x0,y0,x1,y1;
        if (proj(v[a], &x0,&y0) && proj(v[b], &x1,&y1)){
            line(x0,y0,x1,y1,'#');
        }
    }

    // ANSI home (comment out if your terminal doesn't support it)
    printf("\x1b[H");

    // Emit the framebuffer
    for (int y=0;y<H;y++){
        printf("%.*s\r\n", W, &fb[y*W]);
    }
}

int main(void){

    init_usart1(init_clock());

    printf("\033[2J\033[H"); /* clear screen */

    float ax = 0.0f, ay = 0.0f;
    for (;;){
        draw_frame(ax, ay);
        ax += 0.08f;          // tweak speeds for taste
        ay += 0.07f;
        FRAME_DELAY();
    }

    return 0;
}


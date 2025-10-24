
#include "i2c.h"
#include "init.h"
#include "memory.h"
#include "printf.h"
#include "ssd1315.h"
#include "ssd1315_UGUI.h"
#include "ugui.h"
#include "serial.h"
#include <math.h>
#include <stdint.h>

static UG_GUI gui;

int main(void)
{

    clock_info_t ci;
    ci = init_clock();
    init_serial(&ci, 115200);

    init_i2c3_pa8_pc9(&ci);
    init_ssd1315(I2C3);

    ssd1315_set_contrast(1);

    UG_Init(&gui, UG_DrawPixel_SSD1315, SSD1315_W, SSD1315_H);
    UG_FontSelect(&FONT_8X12);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);

    float x=0,r=0;
    int i=0;
    char buf[512] = {};

    /* This renders at about 200fps with I2C clock at about 3.3MHz
       (setting I2C_TIMINGR = 0x00700404 in ssd1315.c) */
    for (;;){

        ssd1315_clear_back();

        sprintf(buf, "HELLOWORLD%04d", i++);
        i%=10000;

        for(int j=0;j<60;j+=10)
            UG_PutString(0,j,buf);

        UG_DrawCircle((int)x,32,(int)r, C_WHITE);
        UG_DrawCircle((int)(x+30.0f),fmod(x,64.0f),(int)r, C_WHITE);

        x += 0.02f;
        r += 0.2f;
        x = fmod(x,127.0f);
        r = fmod(r,37.0f);

        ssd1315_flush();
    }
}


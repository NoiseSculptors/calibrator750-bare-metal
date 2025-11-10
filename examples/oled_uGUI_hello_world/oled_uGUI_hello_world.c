
#include "i2c.h"
#include "init.h"
#include "memory.h"
#include "ssd1315.h"
#include "ssd1315_UGUI.h"
#include "ugui.h"

static UG_GUI gui;

/* ssd1315_printf, a wrapper function combining printf and uGUI UG_PutString
   with buffer on the stack */

int main(void)
{
    clock_info_t ci = init_clock();
    init_i2c3_pa8_pc9(&ci);
    init_ssd1315(I2C3);

    ssd1315_set_contrast(1);

    UG_Init(&gui, UG_DrawPixel_SSD1315, SSD1315_W, SSD1315_H);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);

    for (;;){

        ssd1315_clear_back();

        for(int i=0;i<999;i++){

            UG_FontSelect(&FONT_8X12);
            ssd1315_printf(0,0,"Hello World%03d",i);

            UG_FontSelect(&FONT_6X8);
            int length = ssd1315_printf(0,15,"How long is this line?",i);
            ssd1315_printf(0,35,"%d characters long", length);

            ssd1315_flush(); /* remember to display the image */
        }
    }
}


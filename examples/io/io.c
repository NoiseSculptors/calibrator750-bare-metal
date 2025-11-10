
#include "delay.h"
#include "i2c.h"
#include "init.h"
#include "memory.h"
#include "ssd1315.h"
#include "ssd1315_UGUI.h"
#include "ugui.h"
#include "user_io.h"

int main(void) {

    static UG_GUI gui;

    clock_info_t ci;
    ci = init_clock();

    init_i2c3_pa8_pc9(&ci);
    init_ssd1315(I2C3);

    ssd1315_set_contrast(1);

    UG_Init(&gui, UG_DrawPixel_SSD1315, SSD1315_W, SSD1315_H);
    UG_FontSelect(&FONT_8X12);

    user_io_init();

    uint8_t x=63,white=1;

    for(;;){
        uint32_t dip = user_dipswitch_read(0);
        user_enc_sample_t enc = user_enc_read(0);

        ssd1315_clear_back();

        if(user_btn_read(0)){
            UG_FillCircle(40, 32, 10, white);
        } else {
            UG_DrawCircle(40, 32, 10, white);
        }

        if(user_btn_read(1)) {
            UG_FillCircle(80, 32, 10, white);
        } else {
            UG_DrawCircle(80, 32, 10, white);
        }

        x+=enc.delta;

        if(x>127)
            x=127;
        if(x<3)
            x=3;

        if(x>=90) user_led_write(0,1);
        if(x<90)  user_led_write(0,0);

        if(x>=100) user_led_write(1,1);
        if(x<100)  user_led_write(1,0);

        if(x>=110) user_led_write(2,1);
        if(x<110)  user_led_write(2,0);

        if(x<=127 && x>=3){
            UG_DrawMesh(0,55,x,127, white);
        }

        ssd1315_printf(10,0, "%06b==0x%02x", dip, dip);
        
        ssd1315_flush();
    }
}



/*
   B10 LCD PWR
   B11 LED TIM2_CH4  AF1 
   B12 SPI2_NSS  AF5
   B13 SPI2_SCK  AF5
   B14 RS (DC, DATA/COMMAND)
   B15 SPI2_MOSI AF5
    D8 LCD RESET
 */

#include "delay.h"
#include "init.h"
#include "spi_lcd.h"
#include "rng.h"
#include "user_io.h"
#include <stdint.h>

uint16_t fb[WIDTH*HEIGHT] __attribute__((aligned(32))); 

int main(void){
    init_clock();
    user_io_init();
    init_rng();

    init_pins();
    lcd_hard_reset();
    led_pwm_init(500000);
    led_brightness(30);
    spi_init();
    init_dma_spi();
    fpu_enable();

    sine_lut_fill();
    init_julia_palette();
    lcd_init();


    while(1){
        float x=4.5f;
        while(x<5.2f){
            render_julia(x);
            lcd_flush_fb(fb);
            x+=0.0050f;
        }

        x=0.0f;
        while(x<20.0f){
            render_rotozoom(x);
            lcd_flush_fb(fb);
            x+=0.100f;
        }

        x=0.0f;
        while(x<8.0f){
            render_plasma(x);
            lcd_flush_fb(fb);
            x+=0.100f;
        }

        x=0.0f;
        while(x<20.0f){
            render_tunnel(x);
            lcd_flush_fb(fb);
            x+=0.100f;
        }


        x=0.0f;
        while(x<5.0f){
            for(int i=0;i<WIDTH*HEIGHT; i++)
                fb[i]=(uint16_t)rng_rnd();
            lcd_flush_fb(fb);
            x+=0.100f;
        }

        for(int i=0;i<6;i++){
            display_pattern(i);
            lcd_flush_fb(fb);
            delay_ms(1000);
        }

        /*
           RGB565: RRRRRGGGGGGBBBBB
           in the framebuffer, as cpu is little-endian, pixels are stored: */
        uint16_t
            blue  = 0b0001111100000000,
            green = 0b1110000000000111,
            red   = 0b0000000011111000;

        for(int i=0; i<WIDTH*HEIGHT; i++) fb[i] = red;
        lcd_flush_fb(fb); delay_ms(500);
        for(int i=0; i<WIDTH*HEIGHT; i++) fb[i] = green;
        lcd_flush_fb(fb); delay_ms(500);
        for(int i=0; i<WIDTH*HEIGHT; i++) fb[i] = blue;
        lcd_flush_fb(fb); delay_ms(500);

    }

    return 0;
}


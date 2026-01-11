
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
#include "dwt.h"
#include "init.h"
#include "nvic.h"
#include "spi_lcd.h"
#include "systick.h"
#include "rng.h"
#include "user_io.h"
#include <stdint.h>
#include <string.h>

uint16_t fb[WIDTH*HEIGHT] __attribute__((aligned(32))); 
extern uint8_t oled_buf[1048];

void SysTick_Handler(void)
{
    user_systick_service(); // needed for framebuffer refresh
}

void print_random_text(void){
    char buf[113];
    uint32_t *p;
    p = (uint32_t *)buf;
    for(int i=0;i<28;i++){
        p[i] = rng_rnd();
    }
    fb_printf(0,0,"%s",buf);
}

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
    user_display_init();
    fpu_enable();
    sine_lut_fill();
    init_julia_palette();
    user_display_init();
    lcd_init();
    init_systick_1ms();
    init_dwt();

    for(int i=0;i<100000;i++){
        print_random_text();
    }

    user_display_clear();

    uint32_t ticks;

    while(1){
        float x=4.5f;
        while(x<5.2f){
            dwt_start();
            render_julia(x);
            lcd_flush_fb(fb);
            x+=0.0050f;
            ticks = dwt_stop();
            fb_printf(0,3,"    FPS:%2d    ", 480000000/ticks); 
        }

        x=0.0f;
        while(x<20.0f){
            dwt_start();
            render_rotozoom(x);
            lcd_flush_fb(fb);
            x+=0.100f;
            ticks = dwt_stop();
            fb_printf(4,3,"FPS:%2d ", 480000000/ticks); 
        }

        x=0.0f;
        while(x<8.0f){
            dwt_start();
            render_plasma(x);
            lcd_flush_fb(fb);
            x+=0.100f;
            ticks = dwt_stop();
            fb_printf(4,3,"FPS:%2d ", 480000000/ticks); 
        }

        x=0.0f;
        while(x<20.0f){
            dwt_start();
            render_tunnel(x);
            lcd_flush_fb(fb);
            x+=0.100f;
            ticks = dwt_stop();
            fb_printf(4,3,"FPS:%2d ", 480000000/ticks); 
        }


        x=0.0f;
        while(x<5.0f){
            dwt_start();
            for(int i=0;i<WIDTH*HEIGHT; i++)
                fb[i]=(uint16_t)rng_rnd();
            lcd_flush_fb(fb);
            x+=0.100f;
            ticks = dwt_stop();
            fb_printf(4,3,"FPS:%2d ", 480000000/ticks); 
        }

        for(int i=0;i<6;i++){
            dwt_start();
            display_pattern(i);
            lcd_flush_fb(fb);
            ticks = dwt_stop();
            fb_printf(4,3,"FPS:%2d ", 480000000/ticks); 
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


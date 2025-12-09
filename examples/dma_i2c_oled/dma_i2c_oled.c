
#include "delay.h"
#include "display.h"
#include "dma.h"
#include "dmamux.h"
#include "i2c.h"
#include "init.h"
#include "memory.h"
#include "rcc.h"
#include "rng.h"
#include "ssd1315.h"
#include "systick.h"
#include "user_io.h"
#include <stdint.h>

/* the display is at PA8/PC9 that is I2C3 peripheral */
#define I2CPERIPH I2C3

extern clock_info_t ci;
extern uint8_t oled_buf[OLED_ROWS * STRIDE_SIZE];

static void init_systick(void);
static void init_dma_i2c(uint32_t periph);
static void ssd1315_send_frame_dma_service(uint32_t periph);
static void demo_chars(void);

int main(void)
{

    init_clock();
    init_rng();
    user_io_init();
    init_i2c3_pa8_pc9(&ci);
    init_ssd1315(I2CPERIPH);
    init_dma_i2c(I2CPERIPH);
    ssd1315_set_contrast(1);
    ssd1315_init_dma(I2CPERIPH);
    init_systick();

    /* everything happens in systick handler */
    while(1){
    }

    return 0;
}

/* executes every 1ms */
void SysTick_Handler(void)
{
    static uint32_t counter;
    static uint8_t demo;
    static uint32_t toggle_time_ms;

    if(demo == 0){
        noisesculptors_logo();
    }

    if(demo == 1 && (counter%300) == 0){
        demo_chars();
    }

    if(counter++ == toggle_time_ms){
        oled_clear_framebuffer();
        demo ^= 1;
        if(demo == 0)
            toggle_time_ms = 2000;
        else
            toggle_time_ms = 5000;
        counter = 0;
    }

    ssd1315_send_frame_dma_service(I2CPERIPH);
}

static void ssd1315_send_frame_dma_service(uint32_t periph)
{
    static uint8_t row = 0;
    uint8_t* row_ptr = &oled_buf[row * STRIDE_SIZE];

    /* non-blocking - dma is triggered only if last i2c transaction is finished
     * transfered */
    if (*DMA1_S6CR & 1u)
        return;

    row++;

    *DMA1_HIFCR = (1u<<21)|(1u<<19);
    *DMA1_S6M0AR = (uint32_t)row_ptr;
    *DMA1_S6NDTR = STRIDE_SIZE;
    *DMA1_S6CR |= (1<<0);
    *I2C_CR2(periph) = ((OLED_ADDR<<1)&0x3FF)
                     | (STRIDE_SIZE<<16)
                     | (1u<<25)   // AUTOEND
                     | (1u<<13);  // START

    if (row >= OLED_ROWS)
        row = 0;
}

#define COUNTFLAG 16
#define CLOCK_SRC 2
#define TICKINT   1
#define ENABLE    0

static void init_systick()
{
     *SYSTICK_CTRL = 0;
     *SYSTICK_CTRL |= (1<<CLOCK_SRC)|(1<<TICKINT)|(1<<ENABLE);
     *SYSTICK_LOAD = 480000 - 1;
}

static void init_dma_i2c(uint32_t periph){

    uint8_t dma_request_mux_input;

    /* dma_request_mux_input from table 123. RM0433 */
    if(periph==I2C1)
        dma_request_mux_input = 34;
    else if (periph==I2C2)
        dma_request_mux_input = 36;
    else // (periph==I2C3)
        dma_request_mux_input = 74;

    // Clocks
    *RCC_AHB1ENR |= (1u<<0); // DMA1EN

    // DMAMUX: route I2C2_TX to DMA1 Stream6
    *DMAMUX1_C6CR = 0; // clear first

    *DMAMUX1_C6CR = (dma_request_mux_input & 0x7F);

    // Make sure stream disabled
    *DMA1_S6CR &= ~1u;
    while (*DMA1_S6CR & 1u) {}

    // Addresses
    *DMA1_S6PAR  = (uint32_t)I2C_TXDR(periph); // peripheral = I2C2->TXDR

    // CR: M2P | MINC | 8/8-bit | priority | (optional) TCIE/TEIE
    *DMA1_S6CR = (1u<<10) // MINC=1
               | (1u<<6); // DIR = Memory to peripheral

    // I2C: enable TXDMA
    *I2C_CR1(periph) |= (1u<<14)  // TXDMAEN
                     | (1u<<0);   // enable
}

static void demo_chars(void)
{
    uint8_t cnt=0;
    uint8_t chars[14*8];
    static uint8_t off = 11;

    for(int i=off;i<(14*8) + off; i++)
        chars[cnt++] = i;

    oled_dma_printf(0,0,"%s", chars);

    if(off + (14*8) < 255)
        off++;
    else
        off=11;
}


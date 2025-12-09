
cmake_minimum_required(VERSION 3.16)

add_firmware(
  NAME dma_i2c_oled
  SRCS font.c
       dma_i2c_oled.c
       display.c
       dma.c

  LIBS lib::printf
       driver::ssd1315
       driver::ssd1315_ugui
)


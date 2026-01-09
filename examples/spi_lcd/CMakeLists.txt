
cmake_minimum_required(VERSION 3.19)

add_firmware(
  NAME spi_lcd
  SRCS spi_lcd.c
       spi_lcd_draw.c
       spi_lcd_hw.c
       spi_lcd_julia.c
  LIBS m lib::printf
)



cmake_minimum_required(VERSION 3.19)

add_firmware(
  NAME oled_uGUI_hello_world
  SRCS oled_uGUI_hello_world.c
  LIBS lib::ugui lib::printf driver::ssd1315 driver::ssd1315_ugui
)

target_compile_definitions(oled_uGUI_hello_world PRIVATE
  USE_FONT_8X12
  USE_FONT_6X8
)


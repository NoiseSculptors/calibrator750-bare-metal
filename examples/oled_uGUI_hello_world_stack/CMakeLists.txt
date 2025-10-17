
cmake_minimum_required(VERSION 3.16)

add_firmware(
  NAME oled_uGUI_hello_world_stack
  SRCS oled_uGUI_hello_world_stack.c
  LIBS lib::ugui lib::printf driver::ssd1315 driver::ssd1315_ugui
)



cmake_minimum_required(VERSION 3.19)

set(APP audio_test)

add_firmware(
  NAME ${APP}
  SRCS main.c usb_descriptors.c
  LIBS lib::printf
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")


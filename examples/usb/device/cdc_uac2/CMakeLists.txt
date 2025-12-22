
cmake_minimum_required(VERSION 3.19)

set(APP cdc_uac2)

add_firmware(
  NAME ${APP}
  SRCS cdc_app.c main.c uac2_app.c usb_descriptors.c
  LIBS lib::printf
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")


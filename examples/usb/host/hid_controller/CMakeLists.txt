
cmake_minimum_required(VERSION 3.19)

set(APP hid_controller)

add_firmware(
  NAME ${APP}
  SRCS app.h hid_app.c main.c tusb_config.h
  LIBS lib::printf
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")


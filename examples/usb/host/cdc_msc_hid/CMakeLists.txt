
cmake_minimum_required(VERSION 3.19)

set(APP cdc_msc_hid)

add_firmware(
  NAME ${APP}
  SRCS app.h cdc_app.c hid_app.c main.c msc_app.c tusb_config.h
  LIBS lib::printf
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")


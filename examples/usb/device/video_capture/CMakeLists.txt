
cmake_minimum_required(VERSION 3.19)

set(APP video_capture)

add_firmware(
  NAME ${APP}
  SRCS main.c usb_descriptors.c
  LIBS lib::printf
)

target_compile_definitions(${APP}
    PRIVATE CFG_TUD_ENABLED=1
            CFG_TUH_ENABLED=0
            CFG_TUSB_DEBUG=1
)

noisesculptors_add_tinyusb_for_app(${APP} "${CMAKE_CURRENT_SOURCE_DIR}")


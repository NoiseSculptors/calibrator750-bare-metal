
cmake_minimum_required(VERSION 3.31)

# ---- printf -----------------------------------------------------------------
add_library(lib_printf STATIC printf/printf.c)
add_library(lib::printf ALIAS lib_printf)
target_include_directories(lib_printf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/printf)
target_link_libraries(lib_printf PUBLIC noisesculptors::core)
# Suppress known benign warnings
set_source_files_properties(${CMAKE_SOURCE_DIR}/lib/printf/printf.c
  PROPERTIES COMPILE_OPTIONS "-Werror;-Wno-double-promotion")

# TODO: integrate with SDMMC
# ---- lwext4 -----------------------------------------------------------------
#set(LWEXT4_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lwext4)
#file(GLOB LWEXT4_SRCS ${LWEXT4_DIR}/src/*.c)
#add_library(lib_lwext4 STATIC)
#target_include_directories(lib_lwext4 PUBLIC
#  ${CMAKE_CURRENT_SOURCE_DIR}/lwext4_shim/generated
#  ${CMAKE_CURRENT_SOURCE_DIR}/lwext4_shim
#  ${LWEXT4_DIR}/include)
#target_sources(lib_lwext4 PRIVATE ${LWEXT4_SRCS})
#target_link_libraries(lib_lwext4 PUBLIC noisesculptors::core)
#add_library(lib::lwext4 ALIAS lib_lwext4)

# TODO: integrate with SDMMC
# ---- elm-chan_FatFs ---------------------------------------------------------
#set(ELMCHAN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/elm-chan_FatFs)
#set(ELMCHAN_SHIM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/elm-chan_FatFs_shim)
#add_library(lib_elm-chan_FatFs STATIC)
#target_include_directories(lib_elm-chan_FatFs PUBLIC
#  ${CMAKE_CURRENT_SOURCE_DIR}/elm-chan_FatFs_shim)
#target_sources(lib_elm-chan_FatFs PRIVATE 
#    ${ELMCHAN_SHIM_DIR}/diskio.c # replaces the original diskio.c
#    ${ELMCHAN_DIR}/source/ff.c
#    ${ELMCHAN_DIR}/source/ffsystem.c
#    ${ELMCHAN_DIR}/source/ffunicode.c)
#target_link_libraries(lib_elm-chan_FatFs PUBLIC noisesculptors::core)
#add_library(lib::elm-chan_FatFs ALIAS lib_elm-chan_FatFs)

# ---- lwip -------------------------------------------------------------------
set(LWIP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lwip)
set(LWIP_SHIM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lwip_shim)
add_library(lib_lwip STATIC)
target_include_directories(lib_lwip PUBLIC
  ${CMAKE_SOURCE_DIR}/lib/tinyusb/examples/device/net_lwip_webserver/src
  ${LWIP_DIR}/src/include
)
target_sources(lib_lwip PRIVATE
    ${LWIP_DIR}/src/core/altcp.c
    ${LWIP_DIR}/src/core/altcp_alloc.c
    ${LWIP_DIR}/src/core/altcp_tcp.c
    ${LWIP_DIR}/src/core/def.c
    ${LWIP_DIR}/src/core/dns.c
    ${LWIP_DIR}/src/core/inet_chksum.c
    ${LWIP_DIR}/src/core/init.c
    ${LWIP_DIR}/src/core/ip.c
    ${LWIP_DIR}/src/core/mem.c
    ${LWIP_DIR}/src/core/memp.c
    ${LWIP_DIR}/src/core/netif.c
    ${LWIP_DIR}/src/core/pbuf.c
    ${LWIP_DIR}/src/core/raw.c
    ${LWIP_DIR}/src/core/stats.c
    ${LWIP_DIR}/src/core/sys.c
    ${LWIP_DIR}/src/core/tcp.c
    ${LWIP_DIR}/src/core/tcp_in.c
    ${LWIP_DIR}/src/core/tcp_out.c
    ${LWIP_DIR}/src/core/timeouts.c
    ${LWIP_DIR}/src/core/udp.c
    ${LWIP_DIR}/src/core/ipv4/autoip.c
    ${LWIP_DIR}/src/core/ipv4/dhcp.c
    ${LWIP_DIR}/src/core/ipv4/etharp.c
    ${LWIP_DIR}/src/core/ipv4/icmp.c
    ${LWIP_DIR}/src/core/ipv4/igmp.c
    ${LWIP_DIR}/src/core/ipv4/ip4.c
    ${LWIP_DIR}/src/core/ipv4/ip4_addr.c
    ${LWIP_DIR}/src/core/ipv4/ip4_frag.c
    ${LWIP_DIR}/src/netif/ethernet.c
    ${LWIP_DIR}/src/netif/slipif.c
    ${LWIP_DIR}/src/apps/http/httpd.c
    ${LWIP_DIR}/src/apps/http/fs.c
    ${LWIP_DIR}/src/apps/lwiperf/lwiperf.c
)
target_link_libraries(lib_lwip PUBLIC noisesculptors::core)
add_library(lib::lwip ALIAS lib_lwip)
# ---- UGUI shim --------------------------------------------------------------
set(UGUI_SHIM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/UGUI_shim)

# ---- UGUI (interface-sourced) -----------------------------------------------
# Each consumer compiles UGUI/ugui.c in its own target context.
add_library(lib_ugui INTERFACE)
add_library(lib::ugui ALIAS lib_ugui)

# Give consumers the sources to compile
target_sources(lib_ugui INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/UGUI/ugui.c>
)

target_compile_options(lib_ugui INTERFACE -Wno-missing-prototypes -Wno-parentheses -Wno-discarded-qualifiers)

# Headers/shim; shim should come first in include order
target_include_directories(lib_ugui INTERFACE
  $<BUILD_INTERFACE:${UGUI_SHIM_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/UGUI>
)

# Anything UGUI needs at link/compile time should be INTERFACE so consumers
# inherit it
target_link_libraries(lib_ugui INTERFACE noisesculptors::core)

# (Optional) project-wide defaults hook - apps can ignore this and just set
# their own.
add_library(ugui_defaults INTERFACE)
target_link_libraries(lib_ugui INTERFACE ugui_defaults)

# ---- TinyUSB (device-only, no HAL) ------------------------------------------

# Define once (e.g., in top-level CMakeLists.txt)
set(TINYUSB_SHIM_DIR "${CMAKE_SOURCE_DIR}/lib/tinyusb_shim" CACHE PATH "TinyUSB shim root")
set(TINYUSB_DIR "${CMAKE_SOURCE_DIR}/lib/tinyusb" CACHE PATH "TinyUSB root")

function(noisesculptors_add_tinyusb_for_app APP_TARGET APP_CONFIG_DIR)
  set(TU_DCD "${TINYUSB_DIR}/src/portable/synopsys/dwc2/dcd_dwc2.c")
  if (NOT EXISTS "${TU_DCD}")
    message(FATAL_ERROR "TinyUSB synopsys/dwc2 DCD not found. Update submodule.")
  endif()

  set(TU_SRCS
    ${TINYUSB_SHIM_DIR}/tinyusb_calibrator750.c
    ${TINYUSB_DIR}/lib/networking/rndis_reports.c
    ${TINYUSB_DIR}/lib/networking/dhserver.c
    ${TINYUSB_DIR}/lib/networking/dnserver.c
    ${TINYUSB_DIR}/src/class/vendor/vendor_device.c
    ${TINYUSB_DIR}/src/class/audio/audio_device.c
    ${TINYUSB_DIR}/src/class/audio/audio_device.c
    ${TINYUSB_DIR}/src/class/audio/audio_device.h
    ${TINYUSB_DIR}/src/class/bth/bth_device.c
    ${TINYUSB_DIR}/src/class/cdc/cdc_device.c
    ${TINYUSB_DIR}/src/class/cdc/cdc_host.c
    ${TINYUSB_DIR}/src/class/dfu/dfu_device.c
    ${TINYUSB_DIR}/src/class/dfu/dfu_rt_device.c
    ${TINYUSB_DIR}/src/class/hid/hid_device.c
    ${TINYUSB_DIR}/src/class/hid/hid_host.c
    ${TINYUSB_DIR}/src/class/midi/midi_device.c
    ${TINYUSB_DIR}/src/class/midi/midi_host.c
    ${TINYUSB_DIR}/src/class/msc/msc_device.c
    ${TINYUSB_DIR}/src/class/msc/msc_host.c
    ${TINYUSB_DIR}/src/class/mtp/mtp_device.c
    ${TINYUSB_DIR}/src/class/net/ecm_rndis_device.c
    ${TINYUSB_DIR}/src/class/net/ncm_device.c
    ${TINYUSB_DIR}/src/class/usbtmc/usbtmc_device.c
    ${TINYUSB_DIR}/src/class/video/video_device.c
    ${TINYUSB_DIR}/src/common/tusb_fifo.c
    ${TINYUSB_DIR}/src/device/usbd.c
    ${TINYUSB_DIR}/src/device/usbd_control.c
    ${TINYUSB_DIR}/src/host/hub.c
    ${TINYUSB_DIR}/src/host/usbh.c
    ${TINYUSB_DIR}/src/portable/synopsys/dwc2/dwc2_common.c
    ${TINYUSB_DIR}/src/portable/synopsys/dwc2/hcd_dwc2.c
    ${TINYUSB_DIR}/src/tusb.c
  )

  set(TU_OBJ "tinyusb_${APP_TARGET}_obj")
  add_library(${TU_OBJ} OBJECT
    ${TU_SRCS} ${TU_DEV_SRCS} ${TU_MTP_SRCS} ${TU_DCD}
  )

  # Make TinyUSB OBJECTs inherit all CPU/warn/LTO flags from core
  target_link_libraries(${TU_OBJ} PUBLIC noisesculptors::core)

  # These includes/defs help build the OBJECTs themselves
  target_include_directories(${TU_OBJ} PRIVATE
  # ${APP_CONFIG_DIR}  
  # ${CMAKE_SOURCE_DIR}/lib/noisesculptors-core/include/${PART}
    ${APP_CONFIG_DIR} # tusb_config.h (must come first)
    ${TINYUSB_SHIM_DIR}
    ${TINYUSB_DIR}/src
    ${TINYUSB_DIR}/examples/device/net_lwip_webserver/src
    ${CMAKE_SOURCE_DIR}/lib/lwip/src/include
    ${CMAKE_SOURCE_DIR}/userio
  )

  target_compile_definitions(${TU_OBJ} PRIVATE
    CFG_TUSB_OS=OPT_OS_NONE
    CFG_TUSB_MCU=OPT_MCU_STM32H7
#   $<$<CONFIG:Debug>:CFG_TUSB_DEBUG=2>
  )

  # but they DO NOT propagate to APP_TARGET via $<TARGET_OBJECTS:>.
  # Add them explicitly to the APP target so its own sources (e.g.
  # usb_descriptors.c) see tusb.h.
  target_include_directories(${APP_TARGET} PRIVATE
    ${APP_CONFIG_DIR}
    ${TINYUSB_SHIM_DIR}
    ${TINYUSB_DIR}/src
    ${TINYUSB_DIR}/lib/fatfs/source
    ${TINYUSB_DIR}/lib/embedded-cli
    ${CMAKE_SOURCE_DIR}/lib/lwip/src/include
    ${CMAKE_SOURCE_DIR}/lib/lwip/src/include/lwip/apps
    ${TINYUSB_DIR}/lib/networking
  )

  target_compile_definitions(${APP_TARGET} PRIVATE
    CFG_TUSB_OS=OPT_OS_NONE
    CFG_TUSB_MCU=OPT_MCU_STM32H7
#   $<$<CONFIG:Debug>:CFG_TUSB_DEBUG=2>
  )

  # Finally, add the actual objects to the app
  target_sources(${APP_TARGET} PRIVATE $<TARGET_OBJECTS:${TU_OBJ}>)
endfunction()


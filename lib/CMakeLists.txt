
cmake_minimum_required(VERSION 3.31)

# ---- printf -----------------------------------------------------------------
add_library(lib_printf STATIC printf/printf.c)
add_library(lib::printf ALIAS lib_printf)
target_include_directories(lib_printf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/printf)
target_link_libraries(lib_printf PUBLIC noisesculptors::core)
# Suppress known benign warnings
set_source_files_properties(${CMAKE_SOURCE_DIR}/lib/printf/printf.c
  PROPERTIES COMPILE_OPTIONS "-Werror;-Wno-double-promotion")

# ---- UGUI shim --------------------------------------------------------------
set(UGUI_SHIM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/UGUI_shim)

# ---- UGUI (interface-sourced) -----------------------------------------------
# Each consumer compiles UGUI/ugui.c in its own target context.
add_library(lib_ugui INTERFACE)
add_library(lib::ugui ALIAS lib_ugui)

# Give consumers the sources to compile
target_sources(lib_ugui INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/UGUI/ugui.c>
  # + any other single-translation-unit UGUI sources if needed
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
    ${TINYUSB_DIR}/src/portable/synopsys/dwc2/dwc2_common.c
    ${TINYUSB_DIR}/src/tusb.c
    ${TINYUSB_DIR}/src/common/tusb_fifo.c
    ${TINYUSB_SHIM_DIR}/tinyusb_calibrator750.c
  )

  file(GLOB TU_DEV_SRCS "${TINYUSB_DIR}/src/device/*.c")
  file(GLOB TU_MTP_SRCS "${TINYUSB_DIR}/src/class/mtp/*.c")

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
  -----------------------
    ${APP_CONFIG_DIR} # tusb_config.h (must come first)
    ${TINYUSB_SHIM_DIR}
    ${TINYUSB_DIR}/src
    ${CMAKE_SOURCE_DIR}/userio
  )

  target_compile_definitions(${TU_OBJ} PRIVATE
    CFG_TUSB_OS=OPT_OS_NONE
    CFG_TUSB_MCU=OPT_MCU_STM32H7
    CFG_TUD_ENABLED=1
    CFG_TUH_ENABLED=0
    __RESTRICT=__restrict
    $<$<CONFIG:Debug>:CFG_TUSB_DEBUG=2>
  )

  # but they DO NOT propagate to APP_TARGET via $<TARGET_OBJECTS:>.
  # Add them explicitly to the APP target so its own sources (e.g.
  # usb_descriptors.c) see tusb.h.
  target_include_directories(${APP_TARGET} PRIVATE
    ${APP_CONFIG_DIR}
    ${TINYUSB_SHIM_DIR}
    ${TINYUSB_DIR}/src
  )

  target_compile_definitions(${APP_TARGET} PRIVATE
    CFG_TUSB_OS=OPT_OS_NONE
    CFG_TUSB_MCU=OPT_MCU_STM32H7
    CFG_TUD_ENABLED=1
    CFG_TUH_ENABLED=0
    $<$<CONFIG:Debug>:CFG_TUSB_DEBUG=2>
  )

  target_link_options(${APP_TARGET} PRIVATE
    -specs=nano.specs         # smaller newlib
    -specs=nosys.specs        # provides weak stubs for _exit/_kill/_getpid/etc.
    -Wl,--gc-sections
  )

  # Finally, add the actual objects to the app
  target_sources(${APP_TARGET} PRIVATE $<TARGET_OBJECTS:${TU_OBJ}>)
endfunction()


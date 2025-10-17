
cmake_minimum_required(VERSION 3.16)

project(calibrator750_fw C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Cross-compile QoL: avoid running try-compile executables
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Defaults (override with -D when configuring)
set(BOARD "calibrator750" CACHE STRING "")
set(RUN   "axi"           CACHE STRING "")

# Suffix all executables with .elf
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# Tools for artifacts
find_program(OBJCOPY NAMES arm-none-eabi-objcopy REQUIRED)
find_program(SIZE    NAMES arm-none-eabi-size    REQUIRED)

# Bring core first (defines noisesculptors::core OBJECT target)
add_subdirectory(lib/noisesculptors-core)

# Optional libs/drivers as OBJECTs (kept lean; they should link PUBLIC to core)
add_subdirectory(lib/printf)
add_subdirectory(lib/UGUI)
add_subdirectory(drivers/display/ssd1315)
add_subdirectory(drivers/display/ssd1315_UGUI)

# Helper to create firmware ELF+BIN (linking object libs)
function(add_firmware)
  cmake_parse_arguments(FW "" "NAME" "SRCS;LIBS" ${ARGN})
  if(NOT FW_NAME)
    message(FATAL_ERROR "add_firmware(NAME <name> SRCS <...> [LIBS ...])")
  endif()

  # 1) Target (suffix .elf comes from global setting)
  add_executable(${FW_NAME} ${FW_SRCS})

  # 2) Link object libs (core first), plus any extras the example requests
  target_link_libraries(${FW_NAME} PRIVATE
    noisesculptors::core
    ${FW_LIBS}
  )

  # 3) Linker script from core
  get_target_property(_LDS noisesculptors::core NOISESCULPTORS_LDSCRIPT)
  if(NOT _LDS)
    message(FATAL_ERROR "Core target didn't export NOISESCULPTORS_LDSCRIPT")
  endif()
  get_filename_component(_LDDIR "${_LDS}" DIRECTORY)

  # 4) Linker options (keep link-time LTO via core's PUBLIC -flto)
  target_link_options(${FW_NAME} PRIVATE
    -T${_LDS}
    -Wl,-L${_LDDIR}
    -Wl,--gc-sections
    -Wl,-Map=${FW_NAME}.map
    -nostartfiles
    #  -Wl,--print-memory-usage
    -Wl,--defsym=_end=__bss_end__
  )

  # 5) Post-build .bin and size
  add_custom_command(TARGET ${FW_NAME} POST_BUILD
    COMMAND ${OBJCOPY} -O binary $<TARGET_FILE:${FW_NAME}> ${FW_NAME}.bin
    BYPRODUCTS ${FW_NAME}.bin
    COMMENT "Generating ${FW_NAME}.bin")

  add_custom_command(TARGET ${FW_NAME} POST_BUILD
    COMMAND ${SIZE} $<TARGET_FILE:${FW_NAME}>
    COMMENT "ELF size")
endfunction()

# Examples
add_subdirectory(examples)


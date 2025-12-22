
cmake_minimum_required(VERSION 3.19)

project(prototypes-g0 C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Cross-compile QoL: avoid running try-compile executables
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ---- User knobs -------------------------------------------------------------
# Pick the exact silicon here. Examples: g030, g070, l431, h750
set(PART "g030" CACHE STRING "Exact MCU part (e.g. g030, g070, l431, h750)")

# Run profile (per family): g0x0: flash|sram, l431: flash|sram, h750: flash|axi
set(RUN  "sram" CACHE STRING "Run profile for the chosen family")

# Suffix all executables with .elf
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# Tools for artifacts
find_program(OBJCOPY NAMES arm-none-eabi-objcopy REQUIRED)
find_program(SIZE    NAMES arm-none-eabi-size    REQUIRED)

# ---- Bring the core (defines noisesculptors::core OBJECT target) -----------
add_subdirectory(lib/noisesculptors-core)

# Optional libs/drivers as OBJECTs (kept lean; they should link PUBLIC to core)
add_subdirectory(drivers/display/ssd1315)
add_subdirectory(drivers/display/ssd1315_UGUI)
add_subdirectory(lib)
add_subdirectory(userio)

# ---- Helper to create firmware ELF+BIN (linking object libs) ---------------
function(add_firmware)
  cmake_parse_arguments(FW "" "NAME" "SRCS;LIBS" ${ARGN})
  if(NOT FW_NAME)
    message(FATAL_ERROR "add_firmware(NAME <name> SRCS <...> [LIBS ...])")
  endif()

  # Prefer example-local override, else project-wide, else fall back to weak defaults
  set(FW_ALL_SRCS ${FW_SRCS})
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/user_io.c")
    list(APPEND FW_ALL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/user_io.c")
  elseif(EXISTS "${CMAKE_SOURCE_DIR}/user_io.c")
    list(APPEND FW_ALL_SRCS "${CMAKE_SOURCE_DIR}/user_io.c")
  elseif(EXISTS "${CMAKE_SOURCE_DIR}/userio/user_io.c")
    list(APPEND FW_ALL_SRCS "${CMAKE_SOURCE_DIR}/userio/user_io.c")
  endif()

  add_executable(${FW_NAME} ${FW_ALL_SRCS})

  # Link object libs (core first), plus any extras the example requests
  target_link_libraries(${FW_NAME} PRIVATE
    noisesculptors::core
    lib::userio
    ${FW_LIBS}
  )

  # Linker scripts exported by core: per-part MEMMAP + family LAYOUT
  get_target_property(_LDS    noisesculptors::core NOISESCULPTORS_LDSCRIPT)
  get_target_property(_MEMMAP noisesculptors::core NOISESCULPTORS_MEMMAP)
  if(NOT _LDS OR NOT _MEMMAP)
    message(FATAL_ERROR "Core target did not export NOISESCULPTORS_LDSCRIPT/MEMMAP")
  endif()

  # Linker options (keep link-time LTO via core's PUBLIC -flto)
  target_link_options(${FW_NAME} PRIVATE
    -T${_MEMMAP}          # defines MEMORY {...} for the chosen PART
    -T${_LDS}             # family layout/sections (no MEMORY block inside)
    -Wl,--gc-sections
    -Wl,-Map=${FW_NAME}.map
    -nostartfiles
    # -Wl,--print-memory-usage
    -Wl,--defsym=_end=__bss_end__
    -specs=nano.specs
    -specs=nosys.specs
  )

  # Post-build .bin and size
  add_custom_command(TARGET ${FW_NAME} POST_BUILD
    COMMAND ${OBJCOPY} -O binary $<TARGET_FILE:${FW_NAME}> ${FW_NAME}.bin
    BYPRODUCTS ${FW_NAME}.bin
    COMMENT "Generating ${FW_NAME}.bin")

  add_custom_command(TARGET ${FW_NAME} POST_BUILD
    COMMAND ${SIZE} $<TARGET_FILE:${FW_NAME}>
    COMMENT "ELF size")
endfunction()

# ---- Examples ---------------------------------------------------------------
add_subdirectory(examples)


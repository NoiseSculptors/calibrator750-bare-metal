
cmake_minimum_required(VERSION 3.31)

# ---- printf -----------------------------------------------------------------
add_library(lib_printf STATIC printf/printf.c)
add_library(lib::printf ALIAS lib_printf)
target_include_directories(lib_printf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/printf)
target_link_libraries(lib_printf PUBLIC noisesculptors::core)

# ---- UGUI shim --------------------------------------------------------------
set(UGUI_SHIM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/UGUI_shim)

# ---- UGUI warnings suppression ----------------------------------------------
add_library(ugui_wno INTERFACE)
target_compile_options(ugui_wno INTERFACE
  $<$<AND:$<COMPILE_LANGUAGE:C>,$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>>:
    -Wno-missing-prototypes -Wno-write-strings
  >
)

# ---- UGUI (interface-sourced) -----------------------------------------------
# Each consumer compiles UGUI/ugui.c in its own target context.
add_library(lib_ugui INTERFACE)
add_library(lib::ugui ALIAS lib_ugui)

# Give consumers the sources to compile
target_sources(lib_ugui INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/UGUI/ugui.c>
  # + any other single-translation-unit UGUI sources if needed
)

# Headers/shim; shim should come first in include order
target_include_directories(lib_ugui INTERFACE
  $<BUILD_INTERFACE:${UGUI_SHIM_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/UGUI>
)

# Anything UGUI needs at link/compile time should be INTERFACE so consumers inherit it
target_link_libraries(lib_ugui INTERFACE
  ugui_wno
  noisesculptors::core
)

# (Optional) project-wide defaults hook — apps can ignore this and just set their own.
add_library(ugui_defaults INTERFACE)
target_link_libraries(lib_ugui INTERFACE ugui_defaults)


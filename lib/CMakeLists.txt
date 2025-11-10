
cmake_minimum_required(VERSION 3.31)

# ---- printf -----------------------------------------------------------------
add_library(lib_printf STATIC printf/printf.c)
add_library(lib::printf ALIAS lib_printf)
target_include_directories(lib_printf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/printf)
target_link_libraries(lib_printf PUBLIC noisesculptors::core)
# Suppress known benign warnings
set_source_files_properties(${CMAKE_SOURCE_DIR}/lib/printf/printf.c
  PROPERTIES COMPILE_OPTIONS "-Wno-double-promotion;
                              -Wno-error=double-promotion")

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

# Headers/shim; shim should come first in include order
target_include_directories(lib_ugui INTERFACE
  $<BUILD_INTERFACE:${UGUI_SHIM_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/UGUI>
)

# Anything UGUI needs at link/compile time should be INTERFACE so consumers inherit it
target_link_libraries(lib_ugui INTERFACE
  noisesculptors::core
)

# Suppress known benign warnings
#set_source_files_properties(${CMAKE_SOURCE_DIR}/lib/UGUI/ugui.c
#  PROPERTIES COMPILE_OPTIONS "-Wno-missing-prototypes;
#                              -Wno-parentheses;
#                              -Wno-discarded-qualifiers")

target_compile_options(lib_ugui INTERFACE "-Wno-missing-prototypes;
                                           -Wno-parentheses;
                                           -Wno-discarded-qualifiers")

# (Optional) project-wide defaults hook — apps can ignore this and just set their own.
add_library(ugui_defaults INTERFACE)
target_link_libraries(lib_ugui INTERFACE ugui_defaults)


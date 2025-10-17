
cmake_minimum_required(VERSION 3.16)
project(ext_ugui C)

add_library(lib_ugui OBJECT ugui.c)
add_library(lib::ugui ALIAS lib_ugui)

target_include_directories(lib_ugui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(ugui_wno INTERFACE)

# avoid uGUI warnings
target_compile_options(ugui_wno INTERFACE
  $<$<AND:$<COMPILE_LANGUAGE:C>,$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>>:
    -Wno-missing-prototypes
    -Wno-write-strings
  >
)

target_link_libraries(lib_ugui
  PRIVATE
    noisesculptors::core
    ugui_wno
)


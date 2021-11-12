cmake_minimum_required(VERSION 3.20)
project(main C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

set(headers include/NEO6_M.h)

include_directories(src include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)
set(LIBRARY_OUTPUT_DIRECTORY bin/)

add_library(NEO6_M SHARED src/NEO6_M.c ${headers})

add_executable(main src/main.c ${headers})

target_link_libraries(main NEO6_M serialport)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
cmake_minimum_required(VERSION 2.6)
project(sdl_dev)

set(BIN_DIR ${sdl_dev_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_library(SDL2_LIBRARY NAMES sdl2 HINTS "/usr/local/Cellar")
find_library(SDL2_IMAGE_LIBRARY NAMES sdl2_image HINTS "/usr/local/Cellar")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
add_executable (main src/main.cpp)
target_link_libraries(main ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})

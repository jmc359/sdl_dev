cmake_minimum_required(VERSION 2.6)
project(sdl_dev)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

find_library(SDL2_LIBRARY NAMES sdl2 HINTS "/usr/local/Cellar")
find_library(SDL2_IMAGE_LIBRARY NAMES sdl2_image HINTS "/usr/local/Cellar")

add_library(mylib
	${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/game.hpp
)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME} mylib ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})

cmake_minimum_required(VERSION 2.6)
project(sdl_dev)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

find_library(SDL2_LIBRARY NAMES sdl2 HINTS "/usr/local/Cellar")
find_library(SDL2_IMAGE_LIBRARY NAMES sdl2_image HINTS "/usr/local/Cellar")
find_library(SDL2_TTF_LIBRARY NAMES sdl2_ttf HINTS "/usr/local/Cellar")

add_library(nebula_attack
	${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/agent.cpp
)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} include)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} nebula_attack)

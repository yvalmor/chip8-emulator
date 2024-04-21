cmake_minimum_required(VERSION 3.26)
project(Chip_8_emulator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

add_subdirectory(src)

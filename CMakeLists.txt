cmake_minimum_required(VERSION 3.26)
project(Chip_8_emulator)

set(CMAKE_CXX_STANDARD 23)

add_executable(Chip_8_emulator main.cpp
        Core.h
        opcode.cpp
        opcode.h
)

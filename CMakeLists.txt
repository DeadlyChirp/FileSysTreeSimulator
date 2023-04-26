cmake_minimum_required(VERSION 3.24)
project(project_ccc C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(project_ccc
        src/struct.c
        src/struct.h main.c)

cmake_minimum_required(VERSION 3.0)
project(untitled C)

set(CMAKE_C_STANDARD 99)
add_compile_options("-std=c99")

add_executable(untitled main.c Menu.c Tree.c)

cmake_minimum_required(VERSION 3.20)
project(nickel)

include_directories(src)

set(SOURCES
    src/main.c
    src/common.c
    src/common.h
    src/lexer.c
    src/lexer.h
)

add_executable(nickel ${SOURCES})
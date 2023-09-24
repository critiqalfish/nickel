cmake_minimum_required(VERSION 3.20)
project(nickel)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    src/main.c
    src/common.c
    src/common.h
    src/lexer.c
    src/lexer.h
    src/parser.c
    src/parser.h
)

add_executable(nickel ${SOURCES})
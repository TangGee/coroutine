cmake_minimum_required(VERSION 3.12)
project(coroutine_test C)

set(CMAKE_C_STANDARD 99)

IF(APPLE)
    add_definitions(-D _XOPEN_SOURCE)
ENDIF(APPLE)

add_library(coroutine stack.c coroutine.c)

add_executable(coroutine_test test.c)
target_link_libraries(coroutine_test coroutine)
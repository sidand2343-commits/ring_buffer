cmake_minimum_required(VERSION 3.10)
project(ring_buffer C)

set(CMAKE_C_STANDARD 11)

add_executable(ring_buffer
        ring_buffer.c
        main.c
)
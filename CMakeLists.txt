cmake_minimum_required(VERSION 3.24)
project(bmp_steganography C)

set(CMAKE_C_STANDARD 23)

add_executable(bmp_steganography src/main.c src/bmp_steg.c src/bmp_steg.h src/errors.h src/help.c)

set_target_properties(bmp_steganography PROPERTIES OUTPUT_NAME steg)

//
// Created by joan_ on 21.4.2023 г..
//

#ifndef STEGANOGRAPHY_BMP_STEG_H
#define STEGANOGRAPHY_BMP_STEG_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 5

typedef enum {
    NO_ERROR,
    ERR_OPEN,
    ERR_READ,
    ERR_WRITE,
    ERR_ALLOC,
    ERR_UNSUPPORTED,
    ERR_ENCODE,
    ERR_DECODE,
} err_t;

err_t process(void* buffer, size_t size, FILE* input, FILE* output, uint8_t expect_eof);

err_t process_headers(FILE* input, FILE* output);

void encode(char* message, char* filename);

unsigned char* decode(char* filename);

#endif //STEGANOGRAPHY_BMP_STEG_H

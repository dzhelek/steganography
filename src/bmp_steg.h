//
// Created by joan_ on 21.4.2023 г..
//

#ifndef STEGANOGRAPHY_BMP_STEG_H
#define STEGANOGRAPHY_BMP_STEG_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

#define BUFFER_SIZE 5

typedef struct{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} file_header_t;

typedef struct {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t sizeImage;
    uint32_t xPixelsPerMeter;
    uint32_t yPixelsPerMeter;
    uint32_t clrUsed;
    uint32_t clrImportant;
} info_header_t;

err_t read(void* buffer, size_t size, size_t count, FILE* stream, uint8_t expect_eof);

err_t write(void* buffer, size_t size, size_t count, FILE* stream);

err_t process(void* buffer, size_t size, FILE* input, FILE* output, uint8_t expect_eof);

err_t process_headers(FILE* input, FILE* output);

err_t open(char* filename, char* mode, FILE** p_stream);

void encode(char* message, char* input_filename, char* output_filename);

unsigned char* decode(char* filename);

#endif //STEGANOGRAPHY_BMP_STEG_H

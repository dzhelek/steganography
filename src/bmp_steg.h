//
// Created by joan_ on 21.4.2023 г..
//

#ifndef STEGANOGRAPHY_BMP_STEG_H
#define STEGANOGRAPHY_BMP_STEG_H

#include <stdio.h> // file functions and macros
#include <stdint.h> // integer types
#include <stdlib.h> // dynamic memory management functions
#include <string.h> // strlen, strcpy, strcat

#include "errors.h" // user-defined errors

#define BUFFER_SIZE 5 // the size of the chunks to be decoded at a time

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

// encodes message into the pixel data of the input BMP and writes it into an output BMP
void encode(char* message, char* input_filename, char* output_filename);

// decodes the message from the BMP and returns it
unsigned char* decode(char* filename);

// reads the header data from input file and writes it to the output if given
// also checks if the headers are correct and in the expected format
err_t process_headers(FILE* input, FILE* output);

// reads data from input file and writes it to the output if given
// returns errors on unexpected EOF or other read/write errors
err_t process(void* buffer, size_t size, FILE* input, FILE* output, uint8_t expect_eof);

// fread wrapper, which returns errors if any
err_t read(void* buffer, size_t size, size_t count, FILE* stream, uint8_t expect_eof);

// fwrite wrapper, which returns errors if any
err_t write(void* buffer, size_t size, size_t count, FILE* stream);

// fopen wrapper, which returns errors if any
err_t open(char* filename, char* mode, FILE** p_stream);

#endif //STEGANOGRAPHY_BMP_STEG_H

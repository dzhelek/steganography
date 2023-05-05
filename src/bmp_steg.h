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

/**
 * @brief encodes message into the pixel data of the input BMP and writes it into an output BMP
 * @param message - the text to be encoded
 * @param input_filename - path to the input BMP file
 * @param output_filename - path to the output BMP file
 */
void encode(char* message, char* input_filename, char* output_filename);

/**
 * @brief decodes the message from the BMP and returns it
 * @param filename - path to the BMP file
 * @return the decoded message
 */
unsigned char* decode(char* filename);

/**
 * @brief reads the header data from input file and writes it to the output if given
 *        also checks if the headers are correct and in the expected format
 * @param input - file to read from
 * @param output  - file to write to
 * @return status
 */
err_t process_headers(FILE* input, FILE* output);

/**
 * @brief reads data from input file and writes it to the output if given
 *        returns errors on unexpected EOF or other read/write errors
 * @param buffer - variable to store the data read
 * @param size - number of bytes to be processed
 * @param input - file to read from
 * @param output - file to write to
 * @param expect_eof - flag if EOF is expected
 * @return status
 */
err_t process(void* buffer, size_t size, FILE* input, FILE* output, uint8_t expect_eof);

/**
 * @brief fread wrapper, which returns errors if any
 * @param buffer - variable to store the data read
 * @param size - size of objects to be read
 * @param count - number of objects to be read
 * @param stream - file to read from
 * @param expect_eof - flag if EOF is expected
 * @return status
 */
err_t read(void* buffer, size_t size, size_t count, FILE* stream, uint8_t expect_eof);

/**
 * @brief fwrite wrapper, which returns errors if any
 * @param buffer - variable to be written
 * @param size - size of objects to be written
 * @param count - number of objects to be written
 * @param stream - file to write to
 * @return status
 */
err_t write(void* buffer, size_t size, size_t count, FILE* stream);

/**
 * @brief fopen wrapper, which returns errors if any
 * @param filename - path to file to be opened
 * @param mode - open mode
 * @param p_stream - pointer to a file stream
 * @return status
 */
err_t open(char* filename, char* mode, FILE** p_stream);

#endif //STEGANOGRAPHY_BMP_STEG_H

//
// Created by joan_ on 21.4.2023 г..
//

#include "bmp_steg.h"

struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} file_header;

struct {
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
} info_header;

err_t process(void* buffer, size_t size, FILE* input, FILE* output, uint8_t expect_eof) {
    if (!fread(buffer, size, 1, input)) {
        if (!expect_eof && feof(input)) {
            fprintf(stderr, "ERROR: Unexpected EOF");
            return ERR_READ;
        }
        if (ferror(input)) {
            fprintf(stderr, "Error when reading input file!");
            return ERR_READ;
        }
    }
    if (output != NULL && !fwrite(buffer, size, 1, output)) {
        fprintf(stderr, "Error when writing output file!");
        return ERR_WRITE;
    }
    return NO_ERROR;
}

err_t process_headers(FILE* input, FILE* output) {
    err_t err;
    uint8_t* buffer, length;

    // process file header
    err = process(&file_header.type, 2, input, output, 0);
    if (err) return err;
    if (file_header.type != 0x4d42) {
        fprintf(stderr, "ERROR: unsupported file format");
        return ERR_UNSUPPORTED;
    }
    err = process(&file_header.size, 4, input, output, 0);
    if (err) return err;
    err = process(&file_header.reserved1, 2, input, output, 0);
    if (err) return err;
    err = process(&file_header.reserved2, 2, input, output, 0);
    if (err) return err;
    err = process(&file_header.offset, 4, input, output, 0);
    if (err) return err;


    // process DIB
    err = process(&info_header.size, 4, input, output, 0);
    if (err) return err;
    err = process(&info_header.width, 4, input, output, 0);
    if (err) return err;
    err = process(&info_header.height, 4, input, output, 0);
    if (err) return err;
    err = process(&info_header.planes, 2, input, output, 0);
    if (err) return err;
    err = process(&info_header.bitCount, 2, input, output, 0);
    if (err) return err;
    if (info_header.bitCount != 24) {
        fprintf(stderr, "ERROR: %d-bit images are not supported", info_header.bitCount);
        return ERR_UNSUPPORTED;
    }
    err = process(&info_header.compression, 4, input, output, 0);
    if (err) return err;
    if (info_header.compression) {
        fprintf(stderr, "ERROR: unsupported file compression");
        return ERR_UNSUPPORTED;
    }
    err = process(&info_header.sizeImage, 4, input, output, 0);
    if (err) return err;
    err = process(&info_header.xPixelsPerMeter, 4, input, output, 0);
    if (err) return err;
    err = process(&info_header.yPixelsPerMeter, 4, input, output, 0);
    if (err) return err;
    err = process(&info_header.clrUsed, 4, input, output, 0);
    if (err) return err;
    err = process(&info_header.clrImportant, 4, input, output, 0);
    if (err) return err;

    if ((length = file_header.offset - info_header.size - 14)) {
        buffer = calloc(length, 1);
        err = process(buffer, length, input, output, 0);
        if (err) return err;
    }

    return NO_ERROR;
}

void encode(char* message, char* filename) {
    FILE* input_file;
    FILE* output_file;
    err_t err;
    uint8_t carrier[8];
    uint8_t bits;
    uint64_t mes_i;
    uint64_t length = strlen(message);

    input_file = fopen(filename, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Error when opening input file!");
        exit(ERR_OPEN);
    }
    output_file = fopen("output.bmp", "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Error when creating output file!");
        fclose(input_file);
        exit(ERR_OPEN);
    }

    err = process_headers(input_file, output_file);
    if (file_header.size - file_header.offset < (length+2)*8) {
        fprintf(stderr, "ERROR: unable to encode message of such length in file of such size");
        err = ERR_ENCODE;
    }
    if (err != NO_ERROR){
        fclose(input_file);
        fclose(output_file);
        exit(err);
    }

    fread(carrier, 1, 8, input_file);
    for (bits = 0; bits < 8; bits++) {
        carrier[bits] &= 0xFE;
    }
    fwrite(carrier, 1, 8, output_file);

    for (mes_i = 0; mes_i <= length; mes_i++) {
        fread(carrier, 1, 8, input_file);
        for (bits = 0; bits < 8; bits++) {
            if ((message[mes_i] >> bits) & 1) {
                carrier[bits] |= 1;
            }
            else {
                carrier[bits] &= 0xFE;
            }
        }
        fwrite(carrier, 1, 8, output_file);
    }

    while (!feof(input_file)) {
        process(&bits, 1, input_file, output_file, 1);
    }

    fclose(input_file);
    fclose(output_file);

}

unsigned char* decode(char* filename) {
    FILE* input_file, * output_file;
    err_t err;
    uint64_t mes_i;
    uint8_t bits;
    uint8_t carrier[8];
    uint8_t* message = calloc(BUFFER_SIZE, 1);

    if (message == NULL) {
        fprintf(stderr, "ERROR: not enough dynamic memory");
        exit(ERR_ALLOC);
    }

    input_file = fopen(filename, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Error when opening input file!");
        exit(ERR_OPEN);
    }

    err = process_headers(input_file, NULL);
    if (err != NO_ERROR){
        fclose(input_file);
        exit(err);
    }

    fread(carrier, 1, 8, input_file);
    for (bits = 0; bits < 8; bits++) {
        if (carrier[bits] & 1) {
            fprintf(stderr, "Error when decoding message");
            exit(ERR_DECODE);
        }
    }

    mes_i = 0;
    do {
        if (mes_i == BUFFER_SIZE) {
            message = realloc(message, mes_i + BUFFER_SIZE);
        }
        message[mes_i] = 0;
        fread(carrier, 1, 8, input_file);
        for (bits = 0; bits < 8; bits++) {
            if (carrier[bits] & 1) {
                message[mes_i] |= 1 << bits;
            }
        }
    } while(message[mes_i++]);

    fclose(input_file);

    return message;
}

//
// Created by joan_ on 21.4.2023 г..
//

#include "bmp_steg.h"

file_header_t file_header; // the BMP file header
info_header_t info_header; // the DIB header

void encode(char* message, char* input_filename, char* output_filename) {
    FILE* input_file; // input BMP file
    FILE* output_file; // output BMP file
    err_t err; // error variable
    uint8_t carrier[8]; // a byte, split into its 8 bits
    uint8_t bits; // index/counter variable for the bits of a character
    uint64_t mes_i; // index/counter variable for the message to encode
    uint64_t length = strlen(message); // length of the message

    // open and create the files
    err = open(input_filename, "rb", &input_file);
    if (err) {
        exit(err);
    }
    err = open(output_filename, "wb", &output_file);
    if (err) {
        fclose(input_file);
        exit(err);
    }

    // read and write the headers
    err = process_headers(input_file, output_file);
    if (!err && (file_header.size - file_header.offset < (length+2)*8)) {
        fprintf(stderr, "ERROR: unable to encode message of such length in file of such size");
        err = ERR_ENCODE;
    }
    if (err != NO_ERROR){
        fclose(input_file);
        fclose(output_file);
        remove(output_filename);
        exit(err);
    }

    // read the first 8 bytes from pixel data, set last bits to 0, and write them
    err = read(carrier, 1, 8, input_file, 0);
    if (err) {
        fclose(input_file);
        fclose(output_file);
        remove(output_filename);
        exit(err);
    }
    for (bits = 0; bits < 8; bits++) {
        carrier[bits] &= 0xFE; // set last bits to 0
    }
    err = write(carrier, 1, 8, output_file);
    if (err) {
        fclose(input_file);
        fclose(output_file);
        remove(output_filename);
        exit(err);
    }

    // encode message into the pixel data
    for (mes_i = 0; mes_i <= length; mes_i++) {
        // read 8 bytes
        err = read(carrier, 1, 8, input_file, 0);
        if (err) {
            fclose(input_file);
            fclose(output_file);
            remove(output_filename);
            exit(err);
        }
        // encode message byte into the carrier
        for (bits = 0; bits < 8; bits++) {
            if ((message[mes_i] >> bits) & 1) {
                carrier[bits] |= 1;
            }
            else {
                carrier[bits] &= 0xFE;
            }
        }
        // write modified carrier
        err = write(carrier, 1, 8, output_file);
        if (err) {
            fclose(input_file);
            fclose(output_file);
            remove(output_filename);
            exit(err);
        }
    }

    // read and write remaining part of the file
    while (!feof(input_file)) {
        err = process(&bits, 1, input_file, output_file, 1);
        if (err) {
            fclose(input_file);
            fclose(output_file);
            remove(output_filename);
            exit(err);
        }
    }

    // close files
    fclose(input_file);
    fclose(output_file);
}

unsigned char* decode(char* filename) {
    FILE* input_file; // input BMP file
    err_t err; // error variable
    uint64_t mes_i; // index/counter variable for the message to encode
    uint8_t bits; // index/counter variable for the bits of a character
    uint8_t carrier[8]; // a byte, split into its 8 bits
    uint8_t* message = calloc(BUFFER_SIZE, 1); // variable to store the decoded message into

    if (message == NULL) {
        fprintf(stderr, "ERROR: not enough dynamic memory");
        exit(ERR_ALLOC);
    }

    // open BMP file
    err = open(filename, "rb", &input_file);
    if (err) {
        exit(err);
    }

    // read and check the headers
    err = process_headers(input_file, NULL);
    if (err != NO_ERROR){
        fclose(input_file);
        exit(err);
    }

    // read first 8 bytes from pixel data and check if last bits are 0
    err = read(carrier, 1, 8, input_file, 0);
    if (err) {
        fclose(input_file);
        exit(err);
    }
    for (bits = 0; bits < 8; bits++) {
        if (carrier[bits] & 1) { // check if last bit is 0
            fprintf(stderr, "Error when decoding message");
            fclose(input_file);
            exit(ERR_DECODE);
        }
    }

    // decode message from the pixel data
    mes_i = 0;
    do {
        if (mes_i % BUFFER_SIZE == 0) { // reallocate more memory before buffer overflow
            message = realloc(message, mes_i + BUFFER_SIZE);
            if (message == NULL) {
                fprintf(stderr, "ERROR: not enough dynamic memory");
                fclose(input_file);
                exit(ERR_ALLOC);
            }
        }
        message[mes_i] = 0;
        // read 8 bytes and store every last bit in the respecting position of the message byte
        err = read(carrier, 1, 8, input_file, 0);
        if (err) {
            fclose(input_file);
            exit(err);
        }
        for (bits = 0; bits < 8; bits++) {
            if (carrier[bits] & 1) {
                message[mes_i] |= 1 << bits;
            }
        }
    } while(message[mes_i++]); // repeat until 8 consecutive bytes with 0 in the LSB ('\0')

    // close file
    fclose(input_file);

    return message;
}

err_t process_headers(FILE* input, FILE* output) {
    err_t err; // error variable
    uint8_t* buffer, length; // variables for processing remaining part of the DIB header if any

    // process BMP header
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

    // process DIB header
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

    // process the remaining part of the DIB header if any
    if ((length = file_header.offset - info_header.size - 14)) {
        buffer = calloc(length, 1);
        if (buffer == NULL) {
            fprintf(stderr, "ERROR: not enough dynamic memory");
            return ERR_ALLOC;
        }
        err = process(buffer, length, input, output, 0);
        free(buffer);
        if (err) return err;
    }

    return NO_ERROR;
}

err_t process(void* buffer, size_t size, FILE* input, FILE* output, uint8_t expect_eof) {
    err_t err; // error variable

    err = read(buffer, size, 1, input, expect_eof); // read size data into buffer
    if (err) return err;
    if (output != NULL) { // write size data from buffer into output file if given
        err = write(buffer, size, 1, output);
        if (err) return err;
    }
    return NO_ERROR;
}

err_t read(void* buffer, size_t size, size_t count, FILE* stream, uint8_t expect_eof) {
    if (!fread(buffer, size, count, stream)) { // read data
        if (!expect_eof && feof(stream)) {
            fprintf(stderr, "ERROR: Unexpected EOF");
            return ERR_READ;
        }
        if (ferror(stream)) {
            fprintf(stderr, "Error when reading input file!");
            return ERR_READ;
        }
    }
    return NO_ERROR;
}

err_t write(void* buffer, size_t size, size_t count, FILE* stream) {
    if (!fwrite(buffer, size, count, stream)) { // write data
        fprintf(stderr, "Error when writing output file!");
        return ERR_WRITE;
    }
    return NO_ERROR;
}

err_t open(char* filename, char* mode, FILE** p_stream) {
    *p_stream = fopen(filename, mode); // open file in pointer to the stream
    if (*p_stream == NULL) {
        if (!strcmp(mode, "rb")) {
            fprintf(stderr, "Error when opening input file: %s", filename);
        }
        else {
            fprintf(stderr, "Error when creating output file: %s", filename);
        }
        return ERR_OPEN;
    }

    return NO_ERROR;
}

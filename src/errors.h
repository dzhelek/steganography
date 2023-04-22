//
// Created by joan_ on 22.4.2023 г..
//

#ifndef BMP_STEGANOGRAPHY_ERRORS_H
#define BMP_STEGANOGRAPHY_ERRORS_H

typedef enum {
    NO_ERROR,
    ERR_OPEN,
    ERR_READ,
    ERR_WRITE,
    ERR_ALLOC,
    ERR_UNSUPPORTED,
    ERR_ENCODE,
    ERR_DECODE,
    ERR_COMMAND,
} err_t;

#endif //BMP_STEGANOGRAPHY_ERRORS_H

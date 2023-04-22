//
// Created by joan_ on 18.4.2023 г..
//

#include "bmp_steg.h"

int main(void) {
    char* output_path = "D:\\github";
    char* filename = "hristo.bmp";

    const char* extension = "-steg.bmp";
    uint64_t length = strlen(output_path) + 1 + strlen(filename) - 4 + strlen(extension);
    char* output_filename = calloc(length + 1, 1);
    if (output_filename == NULL) {
        fprintf(stderr, "ERROR: not enough dynamic memory");
        exit(ERR_ALLOC);
    }

    strcat(output_filename, output_path);
    strcat(output_filename, "\\");
    strcat(output_filename, filename);
    output_filename[length - strlen(extension)] = 0;
    strcat(output_filename, extension);

    printf("%s", output_filename);

    char message[] = "Hello World!\nThis message is encoded into an image. Nice, huh?";
    encode(message, filename, output_filename);

    unsigned char* buffer;
    buffer = decode(output_filename);
    printf("\n%s", buffer);

    free(output_filename);

    return 0;
}
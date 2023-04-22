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

    strcpy(output_filename, output_path);
    strcat(output_filename, "\\");
    strcat(output_filename, filename);
    output_filename[length - strlen(extension)] = 0;
    strcat(output_filename, extension);

    printf("%s", output_filename);

    char* message = "Hello World!\nThis message is encoded into an image. Nice, huh?";
    encode(message, filename, output_filename);

    char* text_filename = "hristo.txt";
    unsigned char* buffer;
    err_t err;
    FILE* text_file;
    buffer = decode(output_filename);
    free(output_filename);
    output_filename = calloc(strlen(output_path) + strlen(text_filename) + 2, 1);

    strcpy(output_filename, output_path);
    strcat(output_filename, "\\");
    strcat(output_filename, text_filename);

    err = open(output_filename, "wb", &text_file);
    if (err) {
        exit(err);
    }
    fprintf(text_file, "%s", buffer);
    fclose(text_file);

    free(output_filename);
    free(buffer);

    return 0;
}
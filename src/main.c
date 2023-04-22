//
// Created by joan_ on 18.4.2023 г..
//

#include "bmp_steg.h"

void show_help(void);
void encode_mode(char* message, char* filename, char* output_path);
void decode_mode(char* text_filename, char* filename, char* output_path);

typedef enum {
    NO_MODE,
    MODE_ENCODE,
    MODE_DECODE,
} mode_t;

int main(int argc, char* argv[]) {
    if (argc < 5) {
        fprintf(stderr, "ERROR: Too few arguments!");
        show_help();
        exit(ERR_COMMAND);
    }

    mode_t mode = NO_MODE;
    char* filename = NULL;
    char* path = NULL;
    char* message = NULL;
    char* text_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-o")) {
            path = argv[++i];
        }
        else if (!strcmp(argv[i], "-i")) {
            filename = argv[++i];
        }
        else if (!strcmp(argv[i], "-e")) {
            mode = MODE_ENCODE;
            message = argv[++i];
        }
        else if (!strcmp(argv[i], "-d")) {
            mode = MODE_DECODE;
            text_file = argv[++i];
        }
        else {
            show_help();
            exit(ERR_COMMAND);
        }
    }

    if (filename == NULL) {
        show_help();
        exit(ERR_COMMAND);
    }

    switch (mode) {
        case NO_MODE: {
            show_help();
            exit(ERR_COMMAND);
        }
        case MODE_ENCODE: {
            if (message == NULL) {
                show_help();
                exit(ERR_COMMAND);
            }
            encode_mode(message, filename, path);
            break;
        }
        case MODE_DECODE: {
            if (text_file == NULL) {
                show_help();
                exit(ERR_COMMAND);
            }
            decode_mode(text_file, filename, path);
            break;
        }
    }

    return 0;
}

void encode_mode(char* message, char* filename, char* output_path) {
    const char *extension = "-steg.bmp";
    char* output_filename;
    uint64_t length, path_length;

    if (output_path == NULL) {
        path_length = 0;
    }
    else {
        path_length = strlen(output_path) + 1;
    }

    length = path_length + strlen(filename) - 4 + strlen(extension);
    output_filename = calloc(length + 1, 1);
    if (output_filename == NULL) {
        fprintf(stderr, "ERROR: not enough dynamic memory");
        exit(ERR_ALLOC);
    }

    if (output_path) {
        strcpy(output_filename, output_path);
        strcat(output_filename, "\\");
    }
    strcat(output_filename, filename);
    output_filename[length - strlen(extension)] = 0;
    strcat(output_filename, extension);

    encode(message, filename, output_filename);
    fprintf(stdout, "%s", output_filename);

    free(output_filename);
}

void decode_mode(char* text_filename, char* filename, char* output_path) {
    unsigned char* buffer;
    char* output_filename;
    err_t err;
    FILE* text_file;

    buffer = decode(filename);

    if (output_path == NULL) {
        output_filename = text_filename;
    }
    else {
        output_filename = calloc(strlen(output_path) + strlen(text_filename) + 2, 1);
        if (output_filename == NULL) {
            fprintf(stderr, "ERROR: not enough dynamic memory");
            exit(ERR_ALLOC);
        }

        strcpy(output_filename, output_path);
        strcat(output_filename, "\\");
        strcat(output_filename, text_filename);
    }

    err = open(output_filename, "wb", &text_file);
    if (err) {
        exit(err);
    }
    fprintf(text_file, "%s", buffer);
    fclose(text_file);

    fprintf(stdout, "%s", output_filename);

    free(buffer);
}

void show_help(void) {
    extern unsigned int help_txt_len;
    extern unsigned char help_txt[];

    for (int i = 0; i < help_txt_len; i++) {
        fprintf(stdout, "%c", help_txt[i]);
    }
}

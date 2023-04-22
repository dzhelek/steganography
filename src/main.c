//
// Created by joan_ on 18.4.2023 г..
//

#include "bmp_steg.h" // encode and decode BMP

void show_help(void); // prints info on how to use this tool
void encode_mode(char* message, char* filename, char* output_path); // encodes data into image
void decode_mode(char* text_filename, char* filename, char* output_path); // decodes data from image

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

    mode_t mode = NO_MODE; // program mode - either encode or decode
    char* filename = NULL; // the name of the input BMP image
    char* path = NULL; // path to the output folder
    char* message = NULL; // message to be encoded into image
    char* text_file = NULL; // text file to store the decoded message

    // process command line arguments
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

    // depending on program mode chosen, call the respecting functions
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

void encode_mode(char* message, char* filepath, char* output_path) {
    const char *extension = "-steg.bmp"; // extension to the name of the output BMP image file
    char* output_filename; // path to the output BMP image file
    char* filename; // filename part of the filepath
    uint64_t length, path_length; // lengths of the path both including the filename and not

    // get filename from filepath
    filename = strrchr(filepath, '\\');
    if (filename == NULL){
        filename = filepath;
    }
    else {
        filename++;
    }

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

    // concatenate the output path and the filename of the output BMP image
    if (output_path) {
        strcpy(output_filename, output_path);
        strcat(output_filename, "\\");
    }
    strcat(output_filename, filename);
    output_filename[length - strlen(extension)] = 0; // remove old extension
    strcat(output_filename, extension); // add new extension

    encode(message, filepath, output_filename); // call the encoding function
    fprintf(stdout, "%s", output_filename); // print output file on success

    free(output_filename);
}

void decode_mode(char* text_filename, char* filename, char* output_path) {
    unsigned char* buffer; // buffer to store the decoded message into
    char* output_filename; // path to the output text file
    err_t err; // error variable
    FILE* text_file; // the text file stream

    buffer = decode(filename); // call the decoding function

    // concatenate the output path and the filename of the output text file
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

    // write decoded message into a new text file
    err = open(output_filename, "wb", &text_file);
    if (err) {
        exit(err);
    }
    fprintf(text_file, "%s", buffer);
    fclose(text_file);

    fprintf(stdout, "%s", output_filename); // print output file on success

    free(buffer);
}

void show_help(void) {
    extern unsigned char help_txt[]; // content of the help menu (help.c)
    extern unsigned int help_txt_len; // length of that content

    for (int i = 0; i < help_txt_len; i++) {
        fprintf(stdout, "%c", help_txt[i]); // print the content on the console
    }
}

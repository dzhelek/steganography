//
// Created by joan_ on 18.4.2023 г..
//

#include "bmp_steg.h"

int main(void) {
    char message[] = "Hello World!\nThis message is encoded into an image. Nice, huh?";
    encode(message, "hristo.bmp");

    unsigned char* buffer;
    buffer = decode("output.bmp");
    printf("\n%s", buffer);

    return 0;
}
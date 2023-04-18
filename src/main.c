//
// Created by joan_ on 18.4.2023 г..
//
#include <stdio.h>
#include <stdint.h>

struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offBits;
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

int main(void) {
    FILE* file;
    uint8_t buffer[10];

    file = fopen("pm_extruded_block.bmp", "rb");
    if (file == NULL) {
        fprintf(stderr, "Error when opening file!");
    }
//    printf("%d", sizeof(file_header));
    fread(&file_header.type, 2, 1, file);
    fread(&file_header.size, 4, 1, file);
    fread(&file_header.reserved1, 2, 1, file);
    fread(&file_header.reserved2, 2, 1, file);
    fread(&file_header.offBits, 4, 1, file);

    fread(&info_header.size, 4, 1, file);
    fread(&info_header.width, 4, 1, file);
    fread(&info_header.height, 4, 1, file);
    fread(&info_header.planes, 2, 1, file);
    fread(&info_header.bitCount, 2, 1, file);
    fread(&info_header.compression, 4, 1, file);
    fread(&info_header.sizeImage, 4, 1, file);
    fread(&info_header.xPixelsPerMeter, 4, 1, file);
    fread(&info_header.yPixelsPerMeter, 4, 1, file);
    fread(&info_header.clrUsed, 4, 1, file);
    fread(&info_header.clrImportant, 4, 1, file);

    printf("\nsize: %x", info_header.size);
    printf("\nwidth: %x",info_header.width);
    printf("\nheight: %x",info_header.height);
    printf("\nplanes: %x",info_header.planes);
    printf("\nbit count: %x",info_header.bitCount);
    printf("\ncompression: %x",info_header.compression);
//    for (int i = 0; i < 14; i++) printf("%x ", file_header.bytes[i]);
    fclose(file);

    return 0;
}
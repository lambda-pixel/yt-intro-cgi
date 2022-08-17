#include "bitmap.h"
#include <cstdio>

#pragma pack(1)
struct BMP_Header
{
    uint8_t  type[2];
    uint32_t file_size;
    uint8_t  reserved[4];
    uint32_t offset_start_framebuffer;
};


#pragma pack(1)
struct BMP_Info_Header
{
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t n_color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t raw_size_framebuffer;
    int32_t  h_res;
    int32_t  v_res;
    uint32_t n_color_palettes;
    uint32_t n_important_colors;
};


void save_bmp(const char* filename, BMP_Color framebuffer[], uint32_t width, uint32_t height) 
{
    // Specification des donnees des headers
    const uint32_t size_framebuffer = width * height * sizeof(BMP_Color);

    const BMP_Header bmp_header = {
        .type                     = {'B', 'M'},
        .file_size                = (uint32_t)(sizeof(BMP_Header) + sizeof(BMP_Info_Header) + size_framebuffer),
        .reserved                 = {0},
        .offset_start_framebuffer = sizeof(BMP_Header) + sizeof(BMP_Info_Header)
    };

    const BMP_Info_Header info_header = {
        .header_size          = sizeof(BMP_Info_Header),
        .width                = width,
        .height               = height,
        .n_color_planes       = 1,
        .bits_per_pixel       = 8 * sizeof(BMP_Color),
        .compression_method   = 0,
        .raw_size_framebuffer = size_framebuffer,
        .h_res                = 2835,
        .v_res                = 2835,
        .n_color_palettes     = 0,
        .n_important_colors   = 0
    };

    // Ecriture de l'image dans un fichier
    std::FILE * f_out = std::fopen("test.bmp", "wb");

    if (f_out != NULL) {

        std::fwrite(&bmp_header, sizeof(BMP_Header), 1, f_out);
        std::fwrite(&info_header, sizeof(BMP_Info_Header), 1, f_out);
        std::fwrite(framebuffer, sizeof(BMP_Color), width * height, f_out);

        std::fclose(f_out);
    }
}

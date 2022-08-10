#include <iostream>
#include <cstdint>


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


struct BMP_Color
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t _;
};


int main(int argc, char* argv[])
{
    // Specification de la taille de l'image
    const uint32_t width = 1000;
    const uint32_t height = 600;

    // Specification des donnees des headers
    const uint32_t size_framebuffer = width * height * sizeof(BMP_Color);

    const BMP_Header bmp_header = {
        .type                     = {'B', 'M'},
        .file_size                = sizeof(BMP_Header) + sizeof(BMP_Info_Header) + size_framebuffer,
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

    // Creation de l'image / framebuffer
    BMP_Color *framebuffer = new BMP_Color[width * height];

    for (size_t y = 0; y < height; y++) {
        const uint8_t v = (uint8_t)(255.f * (float)y / (float)(height - 1));
        for (size_t x = 0; x < width; x++) {
            const uint8_t u = (uint8_t)(255.f * (float)x / (float)(width - 1));

            framebuffer[y * width + x].r = v;
            framebuffer[y * width + x].g = u;
            framebuffer[y * width + x].b = 0;
        }
    }

    // Ecriture de l'image dans un fichier
    std::FILE * f_out = std::fopen("test.bmp", "wb");

    if (f_out != NULL) {

        std::fwrite(&bmp_header, sizeof(BMP_Header), 1, f_out);
        std::fwrite(&info_header, sizeof(BMP_Info_Header), 1, f_out);
        std::fwrite(framebuffer, sizeof(BMP_Color), width * height, f_out);

        std::fclose(f_out);
    }  

    delete[] framebuffer;

    return 0;
}
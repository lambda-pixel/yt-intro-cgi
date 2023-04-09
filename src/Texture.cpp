#include "Texture.h"

#include <stdexcept>
#include <cassert>
#include <cstring>

Texture::Texture(int width, int height)
    : _framebuffer(4 * width * height)
    , _width(width)
    , _height(height)
{
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Invalid texture dimensions");
    }
}


void Texture::refresh()
{
    
}

void Texture::put_pixel(int x, int y, Color c)
{
    if (x >= 0  && x < _width && y >= 0 && y < _height) {
        _framebuffer[4 * (y * _width + x) + 0] = c.r;
        _framebuffer[4 * (y * _width + x) + 1] = c.g;
        _framebuffer[4 * (y * _width + x) + 2] = c.b;
        _framebuffer[4 * (y * _width + x) + 3] = 255;   
    } else {
        throw std::runtime_error("Invalid coordinates for writing to texture");
    }
}


Color Texture::get_pixel(int x, int y) const
{
    if (x >= 0  && x < _width && y >= 0 && y < _height) {
        Color ret;
        
        ret.r = _framebuffer[4 * (y * _width + x) + 0];
        ret.g = _framebuffer[4 * (y * _width + x) + 1];
        ret.b = _framebuffer[4 * (y * _width + x) + 2];

        return ret;
    } else {
        throw std::runtime_error("Invalid coordinates for reading texture");
    }

    return {0, 0, 0};
}


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


bool Texture::save(const char* filename) const
{
    // Specification des donnees des headers
    const uint32_t size_framebuffer = _width * _height * 4;

    const BMP_Header bmp_header = {
        /* .type                     = */ {'B', 'M'},
        /* .file_size                = */ (uint32_t)(sizeof(BMP_Header) + sizeof(BMP_Info_Header) + size_framebuffer),
        /* .reserved                 = */ {0},
        /* .offset_start_framebuffer = */ sizeof(BMP_Header) + sizeof(BMP_Info_Header)
    };

    const BMP_Info_Header info_header = {
        /* .header_size          = */ sizeof(BMP_Info_Header),
        /* .width                = */ (uint32_t)_width,
        /* .height               = */ (uint32_t)_height,
        /* .n_color_planes       = */ 1,
        /* .bits_per_pixel       = */ 32,
        /* .compression_method   = */ 0,
        /* .raw_size_framebuffer = */ size_framebuffer,
        /* .h_res                = */ 2835,
        /* .v_res                = */ 2835,
        /* .n_color_palettes     = */ 0,
        /* .n_important_colors   = */ 0
    };

    std::vector<uint8_t> bmp_framebuffer(4 * _width * _height);

    for (int y = 0; y < _height; y++) {
        const int y_bmp = _height - 1 - y;
        
        assert(y_bmp >= 0 && y_bmp < _height);

        for (int x = 0; x < _width; x++) {
            bmp_framebuffer[4 * (y_bmp * _width + x) + 0] = _framebuffer[4 * (y * _width + x) + 2];
            bmp_framebuffer[4 * (y_bmp * _width + x) + 1] = _framebuffer[4 * (y * _width + x) + 1];
            bmp_framebuffer[4 * (y_bmp * _width + x) + 2] = _framebuffer[4 * (y * _width + x) + 0];
        }
    }

    // Ecriture de l'image dans un fichier
    std::FILE * f_out = std::fopen(filename, "wb");

    if (f_out != NULL) {
        std::fwrite(&bmp_header, sizeof(BMP_Header), 1, f_out);
        std::fwrite(&info_header, sizeof(BMP_Info_Header), 1, f_out);
        std::fwrite(bmp_framebuffer.data(), 4, _width * _height, f_out);

        std::fclose(f_out);
    }

    return true;
}
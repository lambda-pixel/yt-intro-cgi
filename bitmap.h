#pragma once

#include <cstdint>

struct BMP_Color
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t _;
};


void save_bmp(const char* filename, BMP_Color framebuffer[], uint32_t width, uint32_t height);

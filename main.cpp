#include <iostream>
#include <cstdint>

#include "bitmap.h"


int main(int argc, char* argv[])
{
    // Specification de la taille de l'image
    const uint32_t width = 1000;
    const uint32_t height = 600;

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

    save_bmp("test.bmp", framebuffer, width, height);

    delete[] framebuffer;

    return 0;
}
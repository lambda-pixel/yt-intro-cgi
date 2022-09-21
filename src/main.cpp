#include <iostream>
#include <cstdint>
#include <cstring>

#include "bitmap.h"
#include "draw.h"


int main(int argc, char* argv[])
{
    // Specification de la taille de l'image
    const uint32_t width = 1000;
    const uint32_t height = 600;

    // Creation de l'image / framebuffer
    BMP_Color *framebuffer = new BMP_Color[width * height];
    std::memset(framebuffer, 0, sizeof(BMP_Color) * width * height);

    for (int x = 0; x < width; x += 30) {
        const float u = (float)x / float(width - 1);

        BMP_Color line_color = {
            .b = (uint8_t)(u * 255.f),
            .g = 255,
            .r = (uint8_t)((1.f - u) * 255.f)
        };

        trace_line(
            framebuffer, width, height, 
            line_color, 
            x, 0,
            width/2, height/2);
    }

    // Enregistrement de l'image
    save_bmp("test.bmp", framebuffer, width, height);

    delete[] framebuffer;

    return 0;
}
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>

#include "bitmap.h"

void trace_line(
    BMP_Color framebuffer[],
    uint32_t width, uint32_t height,
    BMP_Color line_color,
    int x_start, int y_start,
    int x_end, int y_end)
{
    const float dy = (float)(y_end - y_start) / (float)(x_end - x_start);

    if (std::abs(dy) < 1.) {
        if (x_start > x_end) {
            std::swap(x_start, x_end);
            std::swap(y_start, y_end);
        }

        float var_y = 0;

        for (int x = x_start; x <= x_end; x++) {
            int y = std::round((float)y_start + var_y);

            if (x >= 0 && x < width && y >= 0 && y < height) {
                framebuffer[y * width + x] = line_color;
            }

            var_y += dy;
        }
    } else {
        const float dx = (float)(x_end - x_start) / (float)(y_end - y_start);

        if (y_start > y_end) {
            std::swap(x_start, x_end);
            std::swap(y_start, y_end);
        }

        float var_x = 0;

        for (int y = y_start; y <= y_end; y++) {
            int x = std::round((float)x_start + var_x);

            if (x >= 0 && x < width && y >= 0 && y < height) {
                framebuffer[y * width + x] = line_color;
            }

            var_x += dx;
        }
    }
}


int main(int argc, char* argv[])
{
    // Specification de la taille de l'image
    const uint32_t width = 1000;
    const uint32_t height = 600;

    // Creation de l'image / framebuffer
    BMP_Color *framebuffer = new BMP_Color[width * height];
    std::memset(framebuffer, 0, sizeof(BMP_Color) * width * height);

    // Specification de la couleur de la ligne
    BMP_Color green = {
        .b = 0,
        .g = 255,
        .r = 0
    };

    for (int x = 0; x < width; x += 30) {
        trace_line(
            framebuffer, width, height, 
            green, 
            x, 0,
            width/2, height/2);
    }

    // Enregistrement de l'image
    save_bmp("test.bmp", framebuffer, width, height);

    delete[] framebuffer;

    return 0;
}
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>

#include "bitmap.h"


void trace_line_bresenham(
    BMP_Color framebuffer[],
    uint32_t width, uint32_t height,
    BMP_Color line_color,
    int x_start, int y_start,
    int x_end, int y_end)
{
    int dx = x_end - x_start;
    int dy = y_end - y_start;

    if (std::abs(dx) > std::abs(dy)) {
        if (x_start > x_end) {
            std::swap(x_start, x_end);
            std::swap(y_start, y_end);
            dx = -dx;
            dy = -dy;
        }

        int y_inc = 1;

        if (dy < 0) {
            y_inc = -1;
            dy = -dy;
        }

        int y = y_start;
        int D = 2 * dy - dx;

        const int D_inc_1 = 2 * dy;
        const int D_inc_2 = 2 * (dy - dx);

        for (int x = x_start; x <= x_end; x++) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                framebuffer[y * width + x] = line_color;
            }

            if (D < 0) {
                D += D_inc_1;
            } else {
                D += D_inc_2;
                y += y_inc;
            }
        }
    } else {
        if (y_start > y_end) {
            std::swap(x_start, x_end);
            std::swap(y_start, y_end);
            dx = -dx;
            dy = -dy;
        }

        int x_inc = 1;

        if (dx < 0) {
            x_inc = -1;
            dx = -dx;
        }

        int x = x_start;
        int D = 2 * dx - dy;

        const int D_inc_1 = 2 * dx;
        const int D_inc_2 = 2 * (dx - dy);

        for (int y = y_start; y <= y_end; y++) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                framebuffer[y * width + x] = line_color;
            }

            if (D < 0) {
                D += D_inc_1;
            } else {
                D += D_inc_2;
                x += x_inc;
            }
        }
    }
}


void trace_line_dda(
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

    for (int x = 0; x < width; x += 30) {
        const float u = (float)x / float(width - 1);

        BMP_Color line_color = {
            .b = (uint8_t)(u * 255.f),
            .g = 255,
            .r = (uint8_t)((1.f - u) * 255.f)
        };

        trace_line_bresenham(
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
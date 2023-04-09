#include "draw.h"
#include <cstring>
#include <cmath>
#include <algorithm>

void trace_line(
    Texture& texture,
    uint32_t width, uint32_t height,
    Color line_color,
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
            if (x >= 0 && x < (int)width && y >= 0 && y < (int)height) {
                texture.put_pixel(x, y, line_color);
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
            if (x >= 0 && x < (int)width && y >= 0 && y < (int)height) {
                texture.put_pixel(x, y, line_color);
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
    Texture& texture,
    uint32_t width, uint32_t height,
    Color line_color,
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
            int y = (int)std::round((float)y_start + var_y);

            if (x >= 0 && x < (int)width && y >= 0 && y < (int)height) {
                texture.put_pixel(x, y, line_color);
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
            int x = (int)std::round((float)x_start + var_x);

            if (x >= 0 && x < (int)width && y >= 0 && y < (int)height) {
                texture.put_pixel(x, y, line_color);
            }

            var_x += dx;
        }
    }
}
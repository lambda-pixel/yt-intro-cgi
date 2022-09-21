#pragma once

#include <cstdint>
#include <bitmap.h>


void trace_line(
    BMP_Color framebuffer[],
    uint32_t width, uint32_t height,
    BMP_Color line_color,
    int x_start, int y_start,
    int x_end, int y_end);


void trace_line_dda(
    BMP_Color framebuffer[],
    uint32_t width, uint32_t height,
    BMP_Color line_color,
    int x_start, int y_start,
    int x_end, int y_end);
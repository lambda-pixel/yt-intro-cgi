#pragma once

#include <cstdint>
#include <Texture.h>


void trace_line(
    Texture& texture,
    uint32_t width, uint32_t height,
    Color line_color,
    int x_start, int y_start,
    int x_end, int y_end);


void trace_line_dda(
    Texture& texture,
    uint32_t width, uint32_t height,
    Color line_color,
    int x_start, int y_start,
    int x_end, int y_end);
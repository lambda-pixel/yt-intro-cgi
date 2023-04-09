#pragma once

#include <cstdint>
#include <vector>

struct Color
{
    uint8_t r, g, b;
};

class Texture
{
public:
    Texture(int width, int height);

    virtual void refresh();

    virtual void put_pixel(int x, int y, Color c);
    virtual Color get_pixel(int x, int y) const;

    virtual bool save(const char* filename) const;
    
    int width()  const { return _width; }
    int height() const { return _height; }
    
protected:
    std::vector<uint8_t> _framebuffer;
    int _width, _height;
};
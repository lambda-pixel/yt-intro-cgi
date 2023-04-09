#include "SDLTexture.h"
#include <stdexcept>


SDLTexture::SDLTexture(SDL_Renderer* renderer, int width, int height)
    : Texture(width, height)
    , _renderer(renderer)
{
    _texture = SDL_CreateTexture(
        _renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        width, height
    );

    if (_texture == nullptr) {
        throw std::runtime_error("Could not create an SDL texture");
    }
}


SDLTexture::~SDLTexture()
{
    SDL_DestroyTexture(_texture);
}


void SDLTexture::refresh()
{
     SDL_UpdateTexture(
        _texture, NULL,
        _framebuffer.data(),
        4 * _width
    );

    SDL_RenderCopy(_renderer, _texture, NULL, NULL);
}
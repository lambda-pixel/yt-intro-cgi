#pragma once

#include <Texture.h>
#include <SDL.h>

class SDLTexture : public Texture
{
public:
    SDLTexture(SDL_Renderer* renderer, int width, int height);
    ~SDLTexture();

    virtual void refresh() override;

protected:
    SDL_Renderer* _renderer;
    SDL_Texture* _texture;
};
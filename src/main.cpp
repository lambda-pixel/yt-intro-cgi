#include <iostream>
#include <cstdint>
#include <cstring>

#include <SDL.h>

#include "draw.h"
#include "objloader.h"

#include "SDLTexture.h"


void trace_lines_on_texture(
    const std::vector<std::pair<vec4, vec4>>& lines,
    const Color& line_color,
    Texture& texture)
{
    const int width  = texture.width();
    const int height = texture.height();

    for (const auto& edge: lines) {
        trace_line(
            texture, 
            width, height, 
            line_color,
            (int)std::round(edge.first.x), (int)std::round(height - 1 - edge.first.y),
            (int)std::round(edge.second.x), (int)std::round(height - 1 - edge.second.y)
        );       
    }
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage" << std::endl
                  << "-----" << std::endl
                  << argv[0] << " <obj_file>" << std::endl;
        return 0;
    }

    const OBJLoader loader(argv[1]);

    const uint32_t width = 1000;
    const uint32_t height = 600;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow(
        argv[1],
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        0
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (window == nullptr || renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window and renderer %s", SDL_GetError());
        return -1;
    }

    SDLTexture texture(renderer, width, height);

    bool quit = false;

    const Color line_color = {255, 255, 0};

    const std::vector<vec4>& vertices = loader.vertices();
    const BBox bbox = loader.getBBox();

    // Mesh vers NDC
    const float x_center_bbox = (bbox.min.x + bbox.max.x) / 2.f;
    const float y_center_bbox = (bbox.min.y + bbox.max.y) / 2.f;

    const float scaling_ndc_x = 1.f / (bbox.max.x - x_center_bbox);
    const float scaling_ndc_y = 1.f / (bbox.max.y - y_center_bbox);

    const float scaling_ndc = std::min(scaling_ndc_x, scaling_ndc_y);

    const mat4 to_ndc = mat4::u_scale(scaling_ndc) * mat4::x_translate(-x_center_bbox) * mat4::y_translate(-y_center_bbox);

    // NDC vers Image
    const float scaling_img_x = (float)width / 2.f;
    const float scaling_img_y = (float)height / 2.f;

    const float scaling_img = std::min(scaling_img_x, scaling_img_y);

    const float translate_img_x = (float)width / 2.f;
    const float translate_img_y = (float)height / 2.f;

    const mat4 to_image = mat4::x_translate(translate_img_x) * mat4::y_translate(translate_img_y) * mat4::u_scale(scaling_img);

    const mat4 projection = to_image * to_ndc;

    std::vector<std::pair<vec4, vec4>> lines_mesh;

    for (const std::vector<int>& face: loader.polygons()) {
        for (size_t i = 0; i < face.size(); i++) {
            const vec4& s0 = vertices[face[i]];
            const vec4& s1 = vertices[face[(i + 1) % face.size()]];

            lines_mesh.push_back(std::make_pair(s0, s1));
        }
    }

    std::vector<std::pair<vec4, vec4>> lines_screen(lines_mesh.size());
    mat4 model;

    while (!quit) {
        SDL_RenderClear(renderer);

        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_F12:
                        texture.save("screen.bmp");
                        break;

                    case SDLK_LEFT:
                        model = mat4::y_rotate(1.f) * model;
                        break;

                    case SDLK_RIGHT:
                        model = mat4::y_rotate(-1.f) * model;
                        break;

                    case SDLK_DOWN:
                        model = mat4::x_rotate(1.f) * model;
                        break;

                    case SDLK_UP:
                        model = mat4::x_rotate(-1.f) * model;
                        break;

                    default:
                        break;
                }
            default:
                break;
            }
        }

        for (size_t i = 0; i < lines_screen.size(); i++) {
            lines_screen[i].first  = projection * model * lines_mesh[i].first;
            lines_screen[i].second = projection * model * lines_mesh[i].second;
        }

        texture.clear();
        trace_lines_on_texture(lines_screen, line_color, texture);
        texture.refresh();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

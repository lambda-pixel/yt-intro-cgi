#include <iostream>
#include <cstdint>
#include <cstring>

#include <SDL.h>

#include "draw.h"
#include "objloader.h"

#include "SDLTexture.h"


Pnt3D translate_xy(const Pnt3D& pnt, float translate_x, float translate_y)
{
    return {
        pnt.x + translate_x,
        pnt.y + translate_y,
        pnt.z
    };
}


Pnt3D scale(const Pnt3D& pnt, float scale)
{
    return {
        pnt.x * scale,
        pnt.y * scale,
        pnt.z * scale
    };
}


Pnt3D rotate_x(const Pnt3D& pnt, float angle_deg)
{
    const float angle_rad = angle_deg * M_PI/180.f;

    const float cos_angle = std::cos(angle_rad);
    const float sin_angle = std::sin(angle_rad);

    return {
        pnt.x,
        pnt.y * cos_angle - pnt.z * sin_angle,
        pnt.y * sin_angle + pnt.z * cos_angle,
    };
}


Pnt3D rotate_y(const Pnt3D& pnt, float angle_deg)
{
    const float angle_rad = angle_deg * M_PI/180.f;

    const float cos_angle = std::cos(angle_rad);
    const float sin_angle = std::sin(angle_rad);

    return {
        pnt.z * sin_angle + pnt.x * cos_angle,
        pnt.y,
        pnt.z * cos_angle - pnt.x * sin_angle,
    };
}


Pnt3D rotate_z(const Pnt3D& pnt, float angle_deg)
{
    const float angle_rad = angle_deg * M_PI/180.f;

    const float cos_angle = std::cos(angle_rad);
    const float sin_angle = std::sin(angle_rad);

    return {
        pnt.x * cos_angle - pnt.y * sin_angle,
        pnt.x * sin_angle + pnt.y * cos_angle,
        pnt.z
    };
}


void trace_lines_on_texture(
    const std::vector<std::pair<Pnt3D, Pnt3D>>& lines,
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

    const std::vector<Pnt3D>& vertices = loader.vertices();
    const BBox bbox = loader.getBBox();

    std::vector<std::pair<Pnt3D, Pnt3D>> lines;

    for (const std::vector<int>& face: loader.polygons()) {
        for (size_t i = 0; i < face.size(); i++) {
            const Pnt3D& s0 = vertices[face[i]];
            const Pnt3D& s1 = vertices[face[(i + 1) % face.size()]];

            // Mesh vers NDC
            const float x_center_bbox = (bbox.min.x + bbox.max.x) / 2.f;
            const float y_center_bbox = (bbox.min.y + bbox.max.y) / 2.f;

            const float scaling_ndc_x = 1.f / (bbox.max.x - x_center_bbox);
            const float scaling_ndc_y = 1.f / (bbox.max.y - y_center_bbox);

            const float scaling_ndc = std::min(scaling_ndc_x, scaling_ndc_y);

            const Pnt3D s0_ndc = scale(translate_xy(s0, -x_center_bbox, -y_center_bbox), scaling_ndc);
            const Pnt3D s1_ndc = scale(translate_xy(s1, -x_center_bbox, -y_center_bbox), scaling_ndc);

            // NDC vers Image
            const float scaling_img_x = (float)width / 2.f;
            const float scaling_img_y = (float)height / 2.f;

            const float scaling_img = std::min(scaling_img_x, scaling_img_y);

            const float translate_img_x = (float)width / 2.f;
            const float translate_img_y = (float)height / 2.f;

            const Pnt3D s0_img = translate_xy(scale(s0_ndc, scaling_img), translate_img_x, translate_img_y);
            const Pnt3D s1_img = translate_xy(scale(s1_ndc, scaling_img), translate_img_x, translate_img_y);

            lines.push_back(std::make_pair(s0_img, s1_img));
        }
    }

    trace_lines_on_texture(lines, line_color, texture);

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
                        texture.clear();

                        for (auto& edge: lines) {
                            edge.first  = translate_xy(edge.first , -(float)width/2.f, -(float)height / 2.f); 
                            edge.second = translate_xy(edge.second, -(float)width/2.f, -(float)height / 2.f); 

                            edge.first  = rotate_y(edge.first , 1.f);
                            edge.second = rotate_y(edge.second, 1.f);

                            edge.first  = translate_xy(edge.first , (float)width/2.f, (float)height / 2.f); 
                            edge.second = translate_xy(edge.second, (float)width/2.f, (float)height / 2.f); 
                        }

                        trace_lines_on_texture(lines, line_color, texture);
                        break;

                    case SDLK_RIGHT:
                        texture.clear();

                        for (auto& edge: lines) {
                            edge.first  = translate_xy(edge.first , -(float)width/2.f, -(float)height / 2.f); 
                            edge.second = translate_xy(edge.second, -(float)width/2.f, -(float)height / 2.f); 

                            edge.first  = rotate_y(edge.first , -1.f);
                            edge.second = rotate_y(edge.second, -1.f);

                            edge.first  = translate_xy(edge.first , (float)width/2.f, (float)height / 2.f); 
                            edge.second = translate_xy(edge.second, (float)width/2.f, (float)height / 2.f); 
                        }

                        trace_lines_on_texture(lines, line_color, texture);
                        break;

                    case SDLK_DOWN:
                        texture.clear();

                        for (auto& edge: lines) {
                            edge.first  = translate_xy(edge.first , -(float)width/2.f, -(float)height / 2.f); 
                            edge.second = translate_xy(edge.second, -(float)width/2.f, -(float)height / 2.f); 

                            edge.first  = rotate_x(edge.first , 1.f);
                            edge.second = rotate_x(edge.second, 1.f);

                            edge.first  = translate_xy(edge.first , (float)width/2.f, (float)height / 2.f); 
                            edge.second = translate_xy(edge.second, (float)width/2.f, (float)height / 2.f); 
                        }

                        trace_lines_on_texture(lines, line_color, texture);
                        break;

                    case SDLK_UP:
                        texture.clear();

                        for (auto& edge: lines) {
                            edge.first  = translate_xy(edge.first , -(float)width/2.f, -(float)height / 2.f); 
                            edge.second = translate_xy(edge.second, -(float)width/2.f, -(float)height / 2.f); 

                            edge.first  = rotate_x(edge.first , -1.f);
                            edge.second = rotate_x(edge.second, -1.f);

                            edge.first  = translate_xy(edge.first , (float)width/2.f, (float)height / 2.f); 
                            edge.second = translate_xy(edge.second, (float)width/2.f, (float)height / 2.f); 
                        }

                        trace_lines_on_texture(lines, line_color, texture);
                        break;

                    default:
                        break;
                }
            default:
                break;
            }
        }

        texture.refresh();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

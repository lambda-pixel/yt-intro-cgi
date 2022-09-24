#include <iostream>
#include <cstdint>
#include <cstring>

#include "bitmap.h"
#include "draw.h"
#include "objloader.h"


Pnt3D translate_xy(const Pnt3D& pnt, float translate_x, float translate_y)
{
    return {
        pnt.x + translate_x,
        pnt.y + translate_y,
        pnt.z
    };
}


Pnt3D scale(const Pnt3D& pnt, float scale) {
    return {
        pnt.x * scale,
        pnt.y * scale,
        pnt.z * scale
    };
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

    // Specification de la taille de l'image
    const uint32_t width = 1000;
    const uint32_t height = 600;

    // Creation de l'image / framebuffer
    BMP_Color *framebuffer = new BMP_Color[width * height];
    std::memset(framebuffer, 0, sizeof(BMP_Color) * width * height);

    const BMP_Color line_color = {
        .b = 0,
        .g = 255,
        .r = 0
    };

    const std::vector<Pnt3D>& vertices = loader.vertices();
    const BBox bbox = loader.getBBox();

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

            trace_line(
                framebuffer, width, height, line_color,
                s0_img.x, s0_img.y,
                s1_img.x, s1_img.y
            );
        }
    }

    // Enregistrement de l'image
    save_bmp("test.bmp", framebuffer, width, height);

    delete[] framebuffer;

    return 0;
}

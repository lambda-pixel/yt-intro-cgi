#pragma once

#include <array>
#include <cmath>


struct vec4
{
    vec4(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _w = 1.f)
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
    {}

    float x, y, z, w;
};


struct mat4
{
    mat4(
        float r0c0 = 1.f, float r1c0 = 0.f, float r2c0 = 0.f, float r3c0 = 0.f,
        float r0c1 = 0.f, float r1c1 = 1.f, float r2c1 = 0.f, float r3c1 = 0.f,
        float r0c2 = 0.f, float r1c2 = 0.f, float r2c2 = 1.f, float r3c2 = 0.f,
        float r0c3 = 0.f, float r1c3 = 0.f, float r2c3 = 0.f, float r3c3 = 1.f)
    {
        v[0 + 0 * 4] = r0c0;
        v[1 + 0 * 4] = r1c0;
        v[2 + 0 * 4] = r2c0;
        v[3 + 0 * 4] = r3c0;
        
        v[0 + 1 * 4] = r0c1;
        v[1 + 1 * 4] = r1c1;
        v[2 + 1 * 4] = r2c1;
        v[3 + 1 * 4] = r3c1;

        v[0 + 2 * 4] = r0c2;
        v[1 + 2 * 4] = r1c2;
        v[2 + 2 * 4] = r2c2;
        v[3 + 2 * 4] = r3c2;

        v[0 + 3 * 4] = r0c3;
        v[1 + 3 * 4] = r1c3;
        v[2 + 3 * 4] = r2c3;
        v[3 + 3 * 4] = r3c3;
    }


    friend vec4 operator*(const mat4& mat, const vec4& vec)
    {
        vec4 ret;

        for (int r = 0; r < 4; r++) {
            (&ret.x)[r] = 0;

            for (int c = 0; c < 4; c++) {
                (&ret.x)[r] += mat.v[r + c * 4] * (&vec.x)[c];
            }
        }

        return ret;
    }

    friend mat4 operator*(const mat4& mat_l, const mat4& mat_r)
    {
        mat4 ret;

        for (int r = 0; r < 4; r++) {            
            for (int c = 0; c < 4; c++) {
                ret.v[r + c * 4] = 0;

                for (int i = 0; i < 4; i++) {
                    ret.v[r + c * 4] += mat_l.v[r + i * 4] * mat_r.v[i + c * 4];
                }
            }
        }

        return ret;
    }


    static mat4 x_rotate(float angle_deg)
    {
        const float angle_rad = angle_deg * M_PI/180.f;

        const float cos_angle = std::cos(angle_rad);
        const float sin_angle = std::sin(angle_rad);

        return mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, cos_angle, sin_angle, 0.f,
            0.f, -sin_angle, cos_angle, 0.f,
            0.f, 0.f, 0.f, 1.f
        );
    }


    static mat4 y_rotate(float angle_deg)
    {
        const float angle_rad = angle_deg * M_PI/180.f;

        const float cos_angle = std::cos(angle_rad);
        const float sin_angle = std::sin(angle_rad);

        return mat4(
            cos_angle, 0.f, -sin_angle, 0.f,
            0.f, 1.f, 0.f, 0.f,
            sin_angle, 0.f, cos_angle, 0.f,
            0.f, 0.f, 0.f, 1.f
        );
    }


    static mat4 z_rotate(float angle_deg)
    {
        const float angle_rad = angle_deg * M_PI/180.f;

        const float cos_angle = std::cos(angle_rad);
        const float sin_angle = std::sin(angle_rad);

        return mat4(
            cos_angle, sin_angle, 0.f, 0.f,
            -sin_angle, cos_angle, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        );
    }


    static mat4 x_translate(float x)
    {
        return mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            x, 0.f, 0.f, 1.f      
        );
    }


    static mat4 y_translate(float y)
    {
        return mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, y, 0.f, 1.f      
        );
    }


    static mat4 z_translate(float z)
    {
        return mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, z, 1.f      
        );
    }


    static mat4 scale(float x_scale, float y_scale, float z_scale)
    {
        return mat4(
            x_scale, 0.f, 0.f, 0.f,
            0.f, y_scale, 0.f, 0.f,
            0.f, 0.f, z_scale, 0.f,
            0.f, 0.f, 0.f, 1.f      
        );
    }


    static mat4 u_scale(float u_scale)
    {
        return scale(u_scale, u_scale, u_scale);
    }


    std::array<float, 4 * 4> v;
};
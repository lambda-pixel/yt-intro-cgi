#pragma once

#include <vector>
#include <string>


struct Pnt3D {
    float x, y, z, w;
};


struct BBox {
    Pnt3D min;
    Pnt3D max;
};


class OBJLoader
{
public:
    OBJLoader(const char* filename);

    const std::vector<Pnt3D>& vertices() const { return _vertex_buffer; }
    const std::vector<std::vector<int>>& polygons() const { return _index_buffer; }
    BBox getBBox() const { return _bbox; }

private:
    void read_vertex(const std::string& str);
    void read_face_100(const std::string& str);
    void read_face_101(const std::string& str);
    void read_face_111(const std::string& str);

    std::vector<Pnt3D>            _vertex_buffer;
    std::vector<std::vector<int>> _index_buffer;

    BBox _bbox;
};
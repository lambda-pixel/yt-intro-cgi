#pragma once

#include <vector>
#include <string>
#include <regex>

#include <geom.h>

class OBJLoader
{
public:
    OBJLoader(const char* filename);

    const std::vector<vec4>& vertices() const { return _vertex_buffer; }
    const std::vector<std::vector<int>>& polygons() const { return _index_buffer; }
    BBox getBBox() const { return _bbox; }

private:
    void read_vertex(const std::string& str);
    void read_face_100(const std::string& str);
    void read_face_101(const std::string& str);
    void read_face_111(const std::string& str);

    std::vector<vec4>            _vertex_buffer;
    std::vector<std::vector<int>> _index_buffer;

    BBox _bbox;

    static const std::string _float_rgx_str;
    static const std::string _int_rgx_str;

    static const std::string _face_el_100_str;
    static const std::string _face_el_101_str;
    static const std::string _face_el_111_str;

    static const std::regex _vertex_rgx;
    static const std::regex _face_grp_100_rgx;
    static const std::regex _face_grp_101_rgx;
    static const std::regex _face_grp_111_rgx;

    static const std::regex _float_rgx;
    static const std::regex _face_el_100_rgx;
    static const std::regex _face_el_101_rgx;
    static const std::regex _face_el_111_rgx;
};
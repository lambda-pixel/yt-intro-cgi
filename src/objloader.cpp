#include "objloader.h"

#include <fstream>
#include <stdexcept>
#include <regex>
#include <cfloat>


OBJLoader::OBJLoader(const char* filename)
    : _bbox({
        {FLT_MAX, FLT_MAX, FLT_MAX}, 
        {-FLT_MAX, -FLT_MAX, -FLT_MAX}
    })
{
    std::ifstream file_obj(filename, std::ios::in);

    const std::string float_rgx_str = "([+-]?((\\d+([.]\\d*)?)|([.]\\d+))([eE][-+]?\\d+)?)";
    const std::string int_rgx_str   = "([+-]?\\d+)";

    const std::regex vertex_rgx("^v(\\s+" + float_rgx_str + "){3,4}\\s*$");

    const std::string face_grp100 = int_rgx_str;
    const std::string face_grp101 = int_rgx_str + "/{1,2}" + int_rgx_str;
    const std::string face_grp111 = int_rgx_str + "/" + int_rgx_str + "/" + int_rgx_str;

    const std::regex face_grp100_rgx("^f(\\s+" + face_grp100 + "){3,}\\s*$");
    const std::regex face_grp101_rgx("^f(\\s+" + face_grp101 + "){3,}\\s*$");
    const std::regex face_grp111_rgx("^f(\\s+" + face_grp111 + "){3,}\\s*$");

    if (file_obj.is_open()) {

        std::string line;
        std::smatch matches;

        while (std::getline(file_obj, line)) {
            if (std::regex_match(line, matches, vertex_rgx)) {
                read_vertex(line);
            } else if (std::regex_match(line, matches, face_grp100_rgx)) {
                read_face_100(line);
            } else if (std::regex_match(line, matches, face_grp101_rgx)) {
                read_face_101(line);
            } else if (std::regex_match(line, matches, face_grp111_rgx)) {
                read_face_111(line);
            }
        }
        
        file_obj.close();
    } else {
        throw std::runtime_error("Could not load the OBJ file");
    }

    // Boite englobante du mesh
    for (const std::vector<int>& face: _index_buffer) {
        for (const int& index: face) {
            _bbox.min.x = std::min(_bbox.min.x, _vertex_buffer[index].x);
            _bbox.min.y = std::min(_bbox.min.y, _vertex_buffer[index].y);
            _bbox.min.z = std::min(_bbox.min.z, _vertex_buffer[index].z);

            _bbox.max.x = std::max(_bbox.max.x, _vertex_buffer[index].x);
            _bbox.max.y = std::max(_bbox.max.y, _vertex_buffer[index].y);
            _bbox.max.z = std::max(_bbox.max.z, _vertex_buffer[index].z);
        }
    }
}


void OBJLoader::read_vertex(const std::string& str)
{
    const std::regex float_rgx("([+-]?((\\d+([.]\\d*)?)|([.]\\d+))([eE][-+]?\\d+)?)");

    std::string search_str = str;    
    std::smatch matches;

    Pnt3D pnt = {0., 0., 0., 1.};

    for (int i = 0; std::regex_search(search_str, matches, float_rgx); i++) {
        (&pnt.x)[i] = std::stof(matches[0]);
        search_str = matches.suffix().str();
    }

    pnt.x /= pnt.w;
    pnt.y /= pnt.w;
    pnt.z /= pnt.w;
    pnt.w = 1.;

    _vertex_buffer.push_back(pnt);
}


void OBJLoader::read_face_100(const std::string& str)
{
    const std::regex face_grp("([+-]?\\d+)");

    std::string search_str = str;
    std::smatch matches;

    _index_buffer.resize(_index_buffer.size() + 1);
    std::vector<int>& current_polygon = _index_buffer.back();

    while (std::regex_search(search_str, matches, face_grp)) {
        const int vertex_idx = std::stoi(matches[0]);

        if (vertex_idx > 0) {
            current_polygon.push_back(vertex_idx - 1);
        } else {
            current_polygon.push_back(_vertex_buffer.size() - vertex_idx);
        }

        search_str = matches.suffix().str();
    }
}


void OBJLoader::read_face_101(const std::string& str)
{
    const std::string int_rgx_str   = "([+-]?\\d+)";
    const std::regex face_grp(int_rgx_str + "/{1,2}" + int_rgx_str);

    std::string search_str = str;
    std::smatch matches;

    _index_buffer.resize(_index_buffer.size() + 1);
    std::vector<int>& current_polygon = _index_buffer.back();

    while (std::regex_search(search_str, matches, face_grp)) {
        const int vertex_idx = std::stoi(matches[1]);

        if (vertex_idx > 0) {
            current_polygon.push_back(vertex_idx - 1);
        } else {
            current_polygon.push_back(_vertex_buffer.size() - vertex_idx);
        }

        search_str = matches.suffix().str();
    }
}


void OBJLoader::read_face_111(const std::string& str)
{
    const std::string int_rgx_str   = "([+-]?\\d+)";

    const std::regex face_grp(int_rgx_str + "/" + int_rgx_str + "/" + int_rgx_str);

    std::string search_str = str;
    std::smatch matches;

    _index_buffer.resize(_index_buffer.size() + 1);
    std::vector<int>& current_polygon = _index_buffer.back();

    while (std::regex_search(search_str, matches, face_grp)) {
        const int vertex_idx = std::stoi(matches[1]);

        if (vertex_idx > 0) {
            current_polygon.push_back(vertex_idx - 1);
        } else {
            current_polygon.push_back(_vertex_buffer.size() - vertex_idx);
        }

        search_str = matches.suffix().str();
    }
}

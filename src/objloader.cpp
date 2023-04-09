#include "objloader.h"

#include <fstream>
#include <stdexcept>
#include <cfloat>

const std::string OBJLoader::_float_rgx_str = "([+-]?((\\d+([.]\\d*)?)|([.]\\d+))([eE][-+]?\\d+)?)";
const std::string OBJLoader::_int_rgx_str   = "([+-]?\\d+)";

const std::string OBJLoader::_face_el_100_str = OBJLoader::_int_rgx_str;
const std::string OBJLoader::_face_el_101_str = OBJLoader::_int_rgx_str + "/{1,2}" + OBJLoader::_int_rgx_str;
const std::string OBJLoader::_face_el_111_str = OBJLoader::_int_rgx_str + "/" + OBJLoader::_int_rgx_str + "/" + OBJLoader::_int_rgx_str;

const std::regex OBJLoader::_vertex_rgx      ("^v(\\s+" + OBJLoader::_float_rgx_str + "){3,4}\\s*$");
const std::regex OBJLoader::_face_grp_100_rgx("^f(\\s+" + OBJLoader::_face_el_100_str + "){3,}\\s*$");
const std::regex OBJLoader::_face_grp_101_rgx("^f(\\s+" + OBJLoader::_face_el_101_str + "){3,}\\s*$");
const std::regex OBJLoader::_face_grp_111_rgx("^f(\\s+" + OBJLoader::_face_el_111_str + "){3,}\\s*$");

const std::regex OBJLoader::_float_rgx      (OBJLoader::_float_rgx_str);
const std::regex OBJLoader::_face_el_100_rgx(OBJLoader::_face_el_100_str);
const std::regex OBJLoader::_face_el_101_rgx(OBJLoader::_face_el_101_str);
const std::regex OBJLoader::_face_el_111_rgx(OBJLoader::_face_el_111_str);


OBJLoader::OBJLoader(const char* filename)
    : _bbox({
        {FLT_MAX, FLT_MAX, FLT_MAX}, 
        {-FLT_MAX, -FLT_MAX, -FLT_MAX}
    })
{
    std::ifstream file_obj(filename, std::ios::in);

    if (file_obj.is_open()) {

        std::string line;
        std::smatch matches;

        while (std::getline(file_obj, line)) {
            if (std::regex_match(line, matches, _vertex_rgx)) {
                read_vertex(line);
            } else if (std::regex_match(line, matches, _face_grp_100_rgx)) {
                read_face_100(line);
            } else if (std::regex_match(line, matches, _face_grp_101_rgx)) {
                read_face_101(line);
            } else if (std::regex_match(line, matches, _face_grp_111_rgx)) {
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
    std::string search_str = str;    
    std::smatch matches;

    Pnt3D pnt = {0., 0., 0., 1.};

    for (int i = 0; std::regex_search(search_str, matches, _float_rgx); i++) {
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
    std::string search_str = str;
    std::smatch matches;

    _index_buffer.resize(_index_buffer.size() + 1);
    std::vector<int>& current_polygon = _index_buffer.back();

    while (std::regex_search(search_str, matches, _face_el_100_rgx)) {
        const int vertex_idx = std::stoi(matches[0]);

        if (vertex_idx > 0 && vertex_idx <= _vertex_buffer.size()) {
            current_polygon.push_back(vertex_idx - 1);
        } else if ((int)_vertex_buffer.size() - vertex_idx >= 0 
                && (int)_vertex_buffer.size() - vertex_idx < _vertex_buffer.size()) {
            current_polygon.push_back((int)_vertex_buffer.size() - vertex_idx);
        } else {
            std::runtime_error("Malformed OBJ file");
        }

        search_str = matches.suffix().str();
    }
}


void OBJLoader::read_face_101(const std::string& str)
{
    std::string search_str = str;
    std::smatch matches;

    _index_buffer.resize(_index_buffer.size() + 1);
    std::vector<int>& current_polygon = _index_buffer.back();

    while (std::regex_search(search_str, matches, _face_el_101_rgx)) {
        const int vertex_idx = std::stoi(matches[1]);

        if (vertex_idx > 0 && vertex_idx <= _vertex_buffer.size()) {
            current_polygon.push_back(vertex_idx - 1);
        } else if ((int)_vertex_buffer.size() - vertex_idx >= 0 
                && (int)_vertex_buffer.size() - vertex_idx < _vertex_buffer.size()) {
            current_polygon.push_back((int)_vertex_buffer.size() - vertex_idx);
        } else {
            std::runtime_error("Malformed OBJ file");
        }

        search_str = matches.suffix().str();
    }
}


void OBJLoader::read_face_111(const std::string& str)
{
    std::string search_str = str;
    std::smatch matches;

    _index_buffer.resize(_index_buffer.size() + 1);
    std::vector<int>& current_polygon = _index_buffer.back();

    while (std::regex_search(search_str, matches, _face_el_111_rgx)) {
        const int vertex_idx = std::stoi(matches[1]);

        if (vertex_idx > 0 && vertex_idx <= _vertex_buffer.size()) {
            current_polygon.push_back(vertex_idx - 1);
        } else if ((int)_vertex_buffer.size() - vertex_idx >= 0 
                && (int)_vertex_buffer.size() - vertex_idx < _vertex_buffer.size()) {
            current_polygon.push_back((int)_vertex_buffer.size() - vertex_idx);
        } else {
            std::runtime_error("Malformed OBJ file");
        }

        search_str = matches.suffix().str();
    }
}

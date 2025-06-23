#include "terrain.h"
#include "common.h"

Terrain::Terrain(float width, float depth) : _initialized(false), _width(width), _depth(depth) {
    if(width <= 0){
        width = 0.0001f;
    }

    if(depth <= 0){
        depth = 0.0001f;
    }
}

void Terrain::generatePlaneMeshData(){
    _vertices.clear();
    _indices.clear();
    
    _vertices.push_back(Vertex(glm::vec3(-_width/2, 0, -_depth/2), glm::vec3(0, 1, 0), glm::vec2(0, 0)));
    _vertices.push_back(Vertex(glm::vec3( _width/2, 0, -_depth/2), glm::vec3(0, 1, 0), glm::vec2(1, 0)));
    _vertices.push_back(Vertex(glm::vec3(-_width/2, 0,  _depth/2), glm::vec3(0, 1, 0), glm::vec2(0, 1)));
    _vertices.push_back(Vertex(glm::vec3( _width/2, 0,  _depth/2), glm::vec3(0, 1, 0), glm::vec2(1, 1)));
    
    _indices.push_back(0);
    _indices.push_back(2);
    _indices.push_back(1);
    
    _indices.push_back(1);
    _indices.push_back(2);
    _indices.push_back(3);
}

void Terrain::init(void) {
    generatePlaneMeshData();
    _model.load_mesh_from_data(_vertices, _indices);
    _initialized = true;
}

void Terrain::render(void) {
    if (!_initialized)
        init();

    _model.render();
}
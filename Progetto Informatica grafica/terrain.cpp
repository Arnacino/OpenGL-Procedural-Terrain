#include "terrain.h"
#include "common.h"
#include <iostream>

Terrain::Terrain(std::vector<uint8_t> heightMap, glm::vec2 size, std::string textureFileName)
    : _initialized(false), _size(size), _heightMap(heightMap), _textureFileName(textureFileName) {
}


void Terrain::generatePlaneMesh() {
    _vertices.clear();
    _indices.clear();

    unsigned rez = 20;

    //vertici
    for (unsigned i = 0; i <= rez; i++) {
        for (unsigned j = 0; j <= rez; j++) {
            float px = -_size.x / 2.0f + _size.x * i / (float)rez;
            float pz = -_size.y / 2.0f + _size.y * j / (float)rez;
            float u = i / (float)rez;
            float v = j / (float)rez;
            _vertices.push_back(Vertex(glm::vec3(px, 0.0f, pz), glm::vec3(0,1,0), glm::vec2(u,v)));
        }
    }

    //indici
    for (unsigned i = 0; i < rez; i++) {
        for (unsigned j = 0; j < rez; j++) {

            unsigned int topLeft = i * (rez + 1) + j;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (i + 1) * (rez + 1) + j;
            unsigned int bottomRight = bottomLeft + 1;
            
            _indices.push_back(topLeft);     
            _indices.push_back(topRight);    
            _indices.push_back(bottomLeft);  
            _indices.push_back(bottomRight);  
        }
    }
}


void Terrain::init(void) {
    generatePlaneMesh();
    _model.load_mesh_from_data(_vertices, _indices, _textureFileName, _heightMap, _size);
    _initialized = true;
}

void Terrain::render(void) {
    
    if (!_initialized){
        init();
    }    
    _model.render();
}
#include "terrain.h"
#include "common.h"
#include <iostream>

Terrain::Terrain(Noise& noise, std::string textureFileName)
    : _initialized(false), _noise(noise), _textureFileName(textureFileName) {

    _size = _noise.getSize();
}

Terrain::Terrain(const Terrain &other)
    : _initialized(false), _noise(other._noise), _textureFileName(other._textureFileName) {
    _size = other._size;
    // Don't copy _vertices and _indices as they'll be regenerated when init() is called
}

Terrain& Terrain::operator=(const Terrain &other) {
    if (this != &other) {
        // Can't reassign _noise since it's a reference
        _textureFileName = other._textureFileName;
        _size = other._size;
        _initialized = false;  // Force reinitialization
        // Don't copy _vertices and _indices
    }
    return *this;
}

void Terrain::generatePlaneMesh() {
    _vertices.clear();
    _indices.clear();

    //con un valore di 20 otterremo 20*20 patch inizialmente
    unsigned initialDivision = 20;

    //vertici
    for (unsigned i = 0; i <= initialDivision; i++) {
        for (unsigned j = 0; j <= initialDivision; j++) {
            float px = -_size.x / 2.0f + _size.x * i / (float)initialDivision;
            float pz = -_size.y / 2.0f + _size.y * j / (float)initialDivision;
            float u = i / (float)initialDivision;
            float v = j / (float)initialDivision;
            _vertices.push_back(Vertex(glm::vec3(px, 0.0f, pz), glm::vec3(0,1,0), glm::vec2(u,v)));
        }
    }

    //indici
    for (unsigned i = 0; i < initialDivision; i++) {
        for (unsigned j = 0; j < initialDivision; j++) {

            unsigned int topLeft = i * (initialDivision + 1) + j;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (i + 1) * (initialDivision + 1) + j;
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
    _model.load_mesh_from_data(_vertices, _indices, _textureFileName, _noise.getPerlinNoise(), _size);
    _initialized = true;
}

void Terrain::render(void) {
    if (!_initialized){
        init();
    }
    _model.render();

}
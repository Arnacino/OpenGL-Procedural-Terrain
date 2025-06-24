#include "terrain.h"
#include "common.h"
#include <iostream>

Terrain::Terrain(std::vector<uint8_t> heightMap, glm::vec2 size, std::string textureFileName) : _initialized(false), _size(size), _heightMap(heightMap), _textureFileName(textureFileName) {
}

void Terrain::generatePlaneMesh() {
    _vertices.clear();
    _indices.clear();

    float yScale = 32.0f / 256.0f;
    float yShift = 32.0f;

    //genera vertici con normali temporanee
    for(unsigned int i = 0; i < _size.x; i++) {
        for(unsigned int j = 0; j < _size.y; j++) {
            uint8_t y = _heightMap[i*_size.y + j];
            float fx = -_size.x/2.0f + i;
            float fz = -_size.y/2.0f + j;
            float fy = (float)y * yScale - yShift;
            glm::vec2 texCoord(i / (_size.x-1), j / (_size.y-1));
            _vertices.push_back(Vertex(glm::vec3(fx, fy, fz), glm::vec3(0,1,0), texCoord));
        }
    }

    // Genera indici e calcola le normali
    for(unsigned int i = 0; i < _size.x-1; i++) {
        for(unsigned int j = 0; j < _size.y-1; j++) {
            unsigned int row1 = i * _size.y;
            unsigned int row2 = (i+1) * _size.y;

            // Indici per i due triangoli
            unsigned int i1 = row1 + j;
            unsigned int i2 = row2 + j;
            unsigned int i3 = row1 + (j+1);
            unsigned int i4 = row2 + (j+1);

            _indices.push_back(i1);
            _indices.push_back(i3);
            _indices.push_back(i2);

            _indices.push_back(i2);
            _indices.push_back(i3);
            _indices.push_back(i4);
        }
    }

    // Then calculate normals using height differences
    for(unsigned int i = 0; i < _size.x; i++) {
        for(unsigned int j = 0; j < _size.y; j++) {
            // Get height at neighboring points
            float hL = (i > 0) ? _vertices[(i-1)*_size.y + j].position.y : _vertices[i*_size.y + j].position.y;
            float hR = (i < _size.x-1) ? _vertices[(i+1)*_size.y + j].position.y : _vertices[i*_size.y + j].position.y;
            float hD = (j > 0) ? _vertices[i*_size.y + (j-1)].position.y : _vertices[i*_size.y + j].position.y;
            float hU = (j < _size.y-1) ? _vertices[i*_size.y + (j+1)].position.y : _vertices[i*_size.y + j].position.y;
            
            // Calculate the normal using the height differences
            glm::vec3 normal(hL - hR, 2.0f, hD - hU);
            normal = glm::normalize(normal);
            
            // Set the normal for this vertex
            _vertices[i*_size.y + j].normal = normal;
        }
    }
    

}

void Terrain::setHeightMap(std::vector<uint8_t> heightMap){ 
    _heightMap = heightMap;
    _initialized = false;
    render();
}


void Terrain::init(void) {
    generatePlaneMesh();
    _model.load_mesh_from_data(_vertices, _indices, _textureFileName);
    _initialized = true;
}

void Terrain::render(void) {
    if (!_initialized)
        init();

    _model.render();
}
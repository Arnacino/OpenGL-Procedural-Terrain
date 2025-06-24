#include "terrain.h"
#include "common.h"
#include <iostream>

Terrain::Terrain(std::vector<uint8_t> heightMap, glm::vec2 size, std::string textureFileName) : _initialized(false), _size(size), _heightMap(heightMap), _textureFileName(textureFileName) {
}

void Terrain::generatePlaneMesh() {
    _vertices.clear();
    _indices.clear();

    float yScale = 64.0f / 256.0f;
    float yShift = 16.0f;

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

            // Calcola le normali per il primo triangolo
            glm::vec3 v1 = _vertices[i3].position - _vertices[i1].position;
            glm::vec3 v2 = _vertices[i2].position - _vertices[i1].position;
            glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

            // Accumula le normali per ogni vertice
            _vertices[i1].normal += normal;
            _vertices[i2].normal += normal;
            _vertices[i3].normal += normal;

            // Calcola le normali per il secondo triangolo
            v1 = _vertices[i4].position - _vertices[i2].position;
            v2 = _vertices[i3].position - _vertices[i2].position;
            normal = glm::normalize(glm::cross(v1, v2));

            // Accumula le normali per ogni vertice
            _vertices[i3].normal += normal;
            _vertices[i2].normal += normal;
            _vertices[i4].normal += normal;
        }
    }

    // Normalizza tutte le normali accumulate
    for(auto& vertex : _vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }
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
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
    for(unsigned i = 0; i < rez; i++){
        for(unsigned j = 0; j < rez; j++){

            float px = -_size.x/2.0f + _size.x*i/(float)rez;
            float py = 0.0f;
            float pz = -_size.y/2.0f + _size.y*j/(float)rez;
            float u = i / (float) rez;
            float v = j / (float) rez;
            _vertices.push_back(Vertex(glm::vec3(px,py,pz), glm::vec3(0,1,0), glm::vec2(u,v)));

            px = -_size.x/2.0f + _size.x*(i+1)/(float)rez;
            py = 0.0f;
            pz = -_size.y/2.0f + _size.y*j/(float)rez;
            u = (i+1) / (float) rez;
            v = j / (float) rez;
            _vertices.push_back(Vertex(glm::vec3(px,py,pz), glm::vec3(0,1,0), glm::vec2(u,v)));

            px = -_size.x/2.0f + _size.x*i/(float)rez;
            py = 0.0f;
            pz = -_size.y/2.0f + _size.y*(j+1)/(float)rez;
            u = (i+1) / (float) rez;
            v = j / (float) rez;
            _vertices.push_back(Vertex(glm::vec3(px,py,pz), glm::vec3(0,1,0), glm::vec2(u,v)));

            px = -_size.x/2.0f + _size.x*(i+1)/(float)rez;
            py = 0.0f;
            pz = -_size.y/2.0f + _size.y*(j+1)/(float)rez;
            u = (i+1) / (float) rez;
            v = (j+1) / (float) rez;
            _vertices.push_back(Vertex(glm::vec3(px,py,pz), glm::vec3(0,1,0), glm::vec2(u,v)));
        }
    }
    
     //indici
    for(unsigned int i = 0; i < rez; i++) {
        for(unsigned int j = 0; j < rez; j++) {
            // Ogni cella ha 4 vertici, quindi l'indice del primo vertice è (i * rez + j) * 4
            unsigned int cellStart = (i * rez + j) * 4;
            
            // Primo triangolo (vertici 0, 2, 1)
            _indices.push_back(cellStart + 0);
            _indices.push_back(cellStart + 2);
            _indices.push_back(cellStart + 1);
            
            // Secondo triangolo (vertici 1, 2, 3)
            _indices.push_back(cellStart + 1);
            _indices.push_back(cellStart + 2);
            _indices.push_back(cellStart + 3);
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
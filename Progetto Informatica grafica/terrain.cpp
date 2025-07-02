#include "terrain.h"
#include "common.h"
#include <iostream>

Terrain::Terrain(std::vector<uint8_t> noiseData, 
                std::string textureFileName, std::string textureNormalFileName, int size)
    : _initialized(false), _noiseData(noiseData), _textureFileName(textureFileName),
    _textureNormalFileName(textureNormalFileName), _size(size) {}

Terrain::Terrain(const Terrain &other)
    : _initialized(false), _noiseData(other._noiseData), 
    _textureFileName(other._textureFileName), _size(other._size),
    _position(other._position), _textureNormalFileName(other._textureNormalFileName) {}

Terrain& Terrain::operator=(const Terrain &other) {
    if (this != &other) {
        _noiseData = other._noiseData;
        _textureFileName = other._textureFileName;
        _textureNormalFileName = other._textureNormalFileName;
        _size = other._size;
        _position = other._position; 
    }
    return *this;
}

/**
 * position indica la posizione nel mondo dell'oggetto, ovvero "l'offset" dove andrà a posizionarsi l'intera mesh
 * ad esempio position di (20,20) vuol dire che la mesh si posizionera' partendo da (20,20) */
void Terrain::generatePlaneMesh() {
    _vertices.clear();
    _indices.clear();

    //con un valore di 20 otterremo 20*20 patch inizialmente
    unsigned initialDivision = 20;

    float vertexSpacing = _size / (float)initialDivision;

    //vertici
    for (unsigned i = 0; i <= initialDivision; i++) {
        for (unsigned j = 0; j <= initialDivision; j++) {

            float px = _position.x + (vertexSpacing * i);
            float pz = _position.z + (vertexSpacing * j);
            
            float u = (px - _position.x) / _size;
            float v = (pz - _position.z) / _size;
            _vertices.push_back(Vertex(
                glm::vec3(px, 0.0f, pz),
                glm::vec3(0,1,0), 
                glm::vec2(u,v)));
        }
    }

    //indici per le patch di controllo
    for (unsigned i = 0; i < initialDivision; i++) {
        for (unsigned j = 0; j < initialDivision; j++) {

            unsigned int northWest = i * (initialDivision + 1) + j;        // top-left
            unsigned int northEast = northWest + 1;                        // top-right
            unsigned int southWest = (i + 1) * (initialDivision + 1) + j; // bottom-left
            unsigned int southEast = southWest + 1;                        // bottom-right
            
            // Ordine dei vertici di controllo per la patch
            _indices.push_back(northWest);
            _indices.push_back(northEast);
            _indices.push_back(southWest);
            _indices.push_back(southEast);
        }
    }
}


void Terrain::init(void) {
    generatePlaneMesh();
    _model.load_mesh_from_data(_vertices, _indices, _textureFileName, _textureNormalFileName, _noiseData, _size);
    _initialized = true;
}

void Terrain::render(void) {
    if (!_initialized){
        init();
    }
    _model.render();

}
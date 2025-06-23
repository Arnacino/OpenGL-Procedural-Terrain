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
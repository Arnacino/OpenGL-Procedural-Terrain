#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "noise.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Costruttore
Noise::Noise(glm::vec2 size, float scale) 
    : _size(size), _scale(scale) {
    // Verifica che la scala non sia zero
    if (_scale <= 0) {
        _scale = 0.00001f;
    }
}

// Genera il rumore Perlin
void Noise::generate() {
    _noiseData.resize(_size.x * _size.y, 0);
    
    for(int x = 0; x < _size.x; x++) {
        for(int y = 0; y < _size.y; y++) {
            glm::vec2 scaledPos = {x/_scale, y/_scale};
            float n = glm::perlin(scaledPos);
            uint8_t normalizedN = ((n * 0.5f) + 0.5f) * 255;
            _noiseData[y * static_cast<int>(_size.x) + x] = normalizedN;
        }
    }    
}

// Salva l'immagine come file PNG
bool Noise::saveToFile(const std::string& filename) {
    // Genera il rumore se non è già stato generato
    generate();
    
    return stbi_write_png(
        filename.c_str(), 
        static_cast<int>(_size.x), 
        static_cast<int>(_size.y), 
        1, 
        _noiseData.data(), 
        static_cast<int>(_size.x)
    ) != 0;
}

// Getter e setter
glm::vec2 Noise::getSize() const {
    return _size;
}

float Noise::getScale() const {
    return _scale;
}
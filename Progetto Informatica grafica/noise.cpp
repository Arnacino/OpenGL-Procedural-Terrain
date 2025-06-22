#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "noise.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Costruttore
Noise::Noise(glm::vec2 size, float scale, int octave, float persistance, float lacunarity) 
    : _size(size), _scale(scale), _octave(octave), _persistance(persistance), _lacunarity(lacunarity) {
    // evita divisioni per zero
    if (_scale <= 0) {
        _scale = 0.00001f;
    }

    //trova il massimo numero di ottave matematicamente
    int maxOctaves = log2(_size.x < _size.y ? _size.x : _size.y);
    if(_octave <= 0 || _octave > maxOctaves){
        _octave = maxOctaves;
    }
}


/*
Una funzione che genera del perlin noise, sommando i vari segnali ottenuti e utilizzando le variabili
come persistance, lacunarity, scale e octaves per ottenere una singola immagine con valori compresi tra 0 e 255
*/
void Noise::generate() {
    _perlinNoise.resize(_size.x * _size.y, 0);
    
    float minNoise = std::numeric_limits<float>::max();
    float maxNoise = std::numeric_limits<float>::min();
    std::vector<float> rawNoise(_size.x * _size.y, 0);
    
    for(int x = 0; x < _size.x; x++) {
        for(int y = 0; y < _size.y; y++) {
            float amplitude = 1;
            float frequency = 1;
            float noiseHeight = 0;
            
            for(int i = 0; i < _octave; i++){
                glm::vec2 scaledPos = {x / _scale * frequency, y / _scale * frequency};
                float perlinValue = glm::perlin(scaledPos);
                noiseHeight += perlinValue * amplitude;
                amplitude *= _persistance;
                frequency *= _lacunarity;
            }

            rawNoise[y * static_cast<int>(_size.x) + x] = noiseHeight;
            
            // Aggiorna min e max
            minNoise = std::min(minNoise, noiseHeight);
            maxNoise = std::max(maxNoise, noiseHeight);
        }
    }
    
    // Seconda passata per normalizzare e convertire in uint8_t (0-255)
    float range = maxNoise - minNoise;
    for(int i = 0; i < rawNoise.size(); i++) {
        // Normalizza tra 0 e 1
        float normalized = (rawNoise[i] - minNoise) / range;
        // Scala a 0-255 e converte in uint8_t
        _perlinNoise[i] = static_cast<uint8_t>(normalized * 255.0f);
    }
}

bool Noise::saveToFile(const std::string& filename) {

    std::cout<< "generando l'immagine..."<< std::endl;
    generate();
    std::cout<< "immagine generata correttamente."<< std::endl;
    std::cout<< "salvataggio dell'immagine come " << filename << "..."<< std::endl;

    return stbi_write_png(
        filename.c_str(), 
        static_cast<int>(_size.x), 
        static_cast<int>(_size.y), 
        1, 
        _perlinNoise.data(),
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
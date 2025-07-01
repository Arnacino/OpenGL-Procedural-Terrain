#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "noise.h"
#include <random>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <algorithm>

// Costruttore
Noise::Noise(glm::vec2 size, float scale, int octaves, float persistance, float lacunarity, int seed, glm::vec2 offset) 
    : _size(size), _scale(scale), _octaves(octaves), _persistance(persistance), _lacunarity(lacunarity), _seed(seed), _offset(offset) {
    // evita divisioni per zero
    if (_scale <= 0) {
        _scale = 0.00001f;
    }

    //trova il massimo numero di ottave matematicamente
    int maxOctaves = log2(_size.x < _size.y ? _size.x : _size.y);
    if(_octaves <= 0 || _octaves > maxOctaves){
        _octaves = maxOctaves;
    }

    //la dimensione deve essere sempre positiva
    if(_size.x < 0){
        _size.x = 1;
    }

    if(_size.y < 0 || _size.x != _size.y){
        _size.y = _size.x;
    }

    if(_lacunarity < 1){
        _lacunarity = 1;
    }
}


/*
Una funzione che genera del perlin noise, sommando i vari segnali ottenuti e utilizzando le variabili
come persistance, lacunarity, scale e octaves per ottenere una singola immagine con valori compresi tra 0 e 255
*/
void Noise::generate() {

    _perlinNoise.clear();
    _perlinNoise.resize(_size.x * _size.y);
    
    std::vector<glm::vec2> octaveOffset(_octaves);

    float maxPossibleHeight = 0;
    float amplitude = 1;
    float frequency = 1;

    std::mt19937 rng(_seed);
    std::uniform_real_distribution<float> dist(-100000.0f, 100000.0f);

    for(int i = 0; i < _octaves; i++){
        float offsetX = dist(rng) + _offset.x;
        float offsetY = dist(rng) + _offset.y;
        octaveOffset[i] = glm::vec2(offsetX, offsetY);

        maxPossibleHeight += amplitude;
        amplitude *= _persistance;
    }

    float halfX = _size.x / 2.0f;
    float halfY = _size.y / 2.0f;

    for (int y = 0; y < _size.y; y++){
        for (int x = 0; x < _size.x; x++){

            amplitude = 1;
            frequency = 1;
            float noiseHeight = 0;

            for(int i = 0; i < _octaves; i++){
                float sampleX = (x - halfX + octaveOffset[i].x ) / _scale * frequency;
                float sampleY = (y - halfY + octaveOffset[i].y) / _scale * frequency;

                float perlinValue = glm::perlin(glm::vec2(sampleX, sampleY));
                noiseHeight += perlinValue * amplitude;

                amplitude *= _persistance;
                frequency *= _lacunarity;
            }
            
            int index = y * _size.x + x;
            float normalized = (noiseHeight + maxPossibleHeight) / (maxPossibleHeight * 2.0f);
            normalized = std::clamp(normalized, 0.0f, 1.0f);
            _perlinNoise[index] = static_cast<uint8_t>(normalized * 255.0f);
        }   
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

int Noise::getOctaves() const {
    return _octaves;
}

float Noise::getPersistance() const {
        return _persistance;
}

glm::vec2 Noise::getOffset() const {
        return _offset;
}

float Noise::getLacunarity() const {
    return _lacunarity;
}

void Noise::setScale(float scale){
    if(scale > 0){
        _scale = scale;
        _perlinNoise.clear();

    }
}

void Noise::setOctaves(int octaves){
    if(octaves > 0){
        _octaves = octaves;
        _perlinNoise.clear();

    }

}

void Noise::setPersistance(float persistance){
    if(persistance > 0){
        _persistance = persistance;
        _perlinNoise.clear();

    }
}

void Noise::setLacunarity(float lacunarity){
    if(lacunarity > 0){
        _lacunarity = lacunarity;
        _perlinNoise.clear();
    }
}

void Noise::setOffset(glm::vec2 offset){
    _offset = offset;
    _perlinNoise.clear();
}

std::vector<uint8_t> Noise::getPerlinNoise(){
    if(_perlinNoise.empty()){
        Noise::generate();
    }
    return _perlinNoise;
}
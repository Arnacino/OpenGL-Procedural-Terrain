#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>


class Noise {
private:
    glm::vec2 _size;
    float _scale;
    int _octave;
    float _persistance;
    float _lacunarity;
    std::vector<uint8_t> _perlinNoise;

public: 
    Noise(glm::vec2 size, float scale, int octave, float persistance, float lacunarity);
    void generate();
    glm::vec2 getSize() const;
    float getScale() const;
    bool saveToFile(const std::string& filename);
};


#endif // NOISE_H
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
    void generate();

public: 
    Noise(glm::vec2 size, float scale, int octave, float persistance, float lacunarity);
    glm::vec2 getSize() const;

    void setScale(float scale);
    void setOctaves(float octaves);
    void setPersistance(float persistance);
    void setLacunarity(float lacunarity);
    float getScale() const;
    float getOctaves() const;
    float getPersistance() const;
    float getLacunarity() const;
    
    std::vector<uint8_t> getPerlinNoise();
    bool saveToFile(const std::string& filename);
};


#endif // NOISE_H
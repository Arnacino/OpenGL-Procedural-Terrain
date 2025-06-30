#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>


class Noise {
private:
    glm::vec2 _size;
    float _scale;
    int _octaves;
    float _persistance;
    float _lacunarity;
    int _seed;
    glm::vec2 _offset;
    std::vector<uint8_t> _perlinNoise;
    void generate();

public: 
    Noise(glm::vec2 size, float scale, int octaves, float persistance, float lacunarity, int seed, glm::vec2 offset);
    glm::vec2 getSize() const;

    void setScale(float scale);
    void setOctaves(int octaves);
    void setPersistance(float persistance);
    void setLacunarity(float lacunarity);
    void setOffset(glm::vec2 offset);
    float getScale() const;
    glm::vec2 getOffset() const;
    int getOctaves() const;
    float getPersistance() const;
    float getLacunarity() const;
    
    std::vector<uint8_t> getPerlinNoise();
    bool saveToFile(const std::string& filename);
};


#endif // NOISE_H
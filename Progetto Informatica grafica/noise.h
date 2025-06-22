#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>


class Noise {
private:
    glm::vec2 _size;
    float _scale;
    std::vector<uint8_t> _noiseData;

public: 
    Noise(glm::vec2 size, float scale);
    void generate();
    glm::vec2 getSize() const;
    float getScale() const;
    bool saveToFile(const std::string& filename);
};


#endif // NOISE_H
#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

/**
 * Classe che genera Fractal Perlin Noise, ovvero delle somme (octaves) di Perlin noise.
 * Viene utilizzata per generare heightmap procedurali per il terreno.
 * 
 * Parametri di input:
 * - size: dimensione dell'array di perlin noise (deve essere quadrata, size.x = size.y)
 * - scale: fattore di scala del noise. Valori più alti producono features più grandi
 * - octaves: numero di layers di noise sovrapposti. Più ottave = più dettagli
 * - persistance: quanto ogni ottava influenza l'ampiezza. Range (0,1)
 * - lacunarity: quanto ogni ottava influenza la frequenza. Deve essere > 1
 * - seed: seed per la generazione random degli offset
 * - offset: offset globale per lo spostamento del noise
 * 
 * I valori generati sono normalizzati nel range [0,1].
 */
class Noise {
private:
    glm::vec2 _size;
    float _scale;
    int _octaves;
    float _persistance;
    float _lacunarity;
    int _seed;
    glm::vec2 _offset;
    std::vector<float> _perlinNoise;
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
    
    std::vector<float> getPerlinNoise();
};


#endif // NOISE_H
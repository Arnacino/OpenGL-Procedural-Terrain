#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "glm/glm.hpp"
#include <unordered_map>
#include "terrain.h"
#include "noise.h"


struct Vec2Hash {
    std::size_t operator()(const glm::ivec2& v) const {
        std::hash<float> hasher;
        size_t h1 = hasher(v.x);
        size_t h2 = hasher(v.y);
        return h1 ^ (h2 << 1); // XOR combinato
    }
};

// Comparatore di uguaglianza per glm::vec2
struct Vec2Equal {
    bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

class ChunkManager {

private:
    Noise& _noise;
    float _maxViewDistance;
    int _chunkSize;
    int _chunkVisibleInViewDistance;
    glm::vec3 _cameraPosition;
    std::string _chunksTextureFileName;
    std::string _chunksTextureNormalFileName;
    std::unordered_map<glm::ivec2, Terrain, Vec2Hash, Vec2Equal> _chunks;
    void generateChunk(glm::ivec2 coords);
    bool chunkExists(glm::ivec2 coords) const;
    void unloadDistantChunks();
    
public:
    ChunkManager(float maxViewDistance, glm::vec3 cameraPosition, 
        Noise& noise, std::string chunksTextureFileName, 
        std::string chunksTextureNormalFileName);
    void update(glm::vec3 cameraPosition);
    void render();
    void setCameraPos(glm::vec3 cameraPosition){ 
        _cameraPosition = cameraPosition;
    }
        // Add this method to access chunks
    const std::unordered_map<glm::ivec2, Terrain, Vec2Hash, Vec2Equal>& getChunks() const {
        return _chunks;
    }
};

#endif
#include "chunkManager.h"
#include <iostream>

ChunkManager::ChunkManager(float maxViewDistance, glm::vec3 cameraPosition, 
    Noise& noise, std::string chunkTextureFileName)
    : _maxViewDistance(maxViewDistance), _cameraPosition(cameraPosition),
        _noise(noise), _chunksTextureFileName(chunkTextureFileName) {
        _chunkSize = _noise.getSize().x;
        _chunkVisibleInViewDistance = (int) _maxViewDistance / _chunkSize;
    }

void ChunkManager::update(glm::vec3 cameraPosition) {
    setCameraPos(cameraPosition);
    unloadDistantChunks();
    int currentChunkX = static_cast<int>(std::floor(cameraPosition.x / _chunkSize));
    int currentChunkZ = static_cast<int>(std::floor(cameraPosition.z / _chunkSize));

    for(int zOffset = -_chunkVisibleInViewDistance; zOffset <= _chunkVisibleInViewDistance; zOffset++){
        for(int xOffset = -_chunkVisibleInViewDistance; xOffset <= _chunkVisibleInViewDistance; xOffset++){
            glm::ivec2 viewedChunkCoord = glm::ivec2(currentChunkX + xOffset, currentChunkZ + zOffset);
            if(!chunkExists(viewedChunkCoord)){
                generateChunk(viewedChunkCoord);
            }
        }
    }
}

void ChunkManager::generateChunk(glm::ivec2 coords){
    _noise.setOffset(glm::vec2(coords.x * _chunkSize/2, coords.y * _chunkSize/2));
    std::vector<uint8_t> noiseData = _noise.getPerlinNoise();
    std::cout<<"Generazione del chunk " << coords.x << ", " << coords.y <<" con offset: " << _noise.getOffset().x << ", " << _noise.getOffset().y<< std::endl;
    _noise.saveToFile(std::to_string(coords.x*_chunkSize) +","+ std::to_string(coords.y*_chunkSize) + ".jpg");
    Terrain chunk = Terrain(noiseData, _chunksTextureFileName, _noise.getSize().x);
    chunk.setPosition(glm::vec3(coords.x * _chunkSize, 0.0f, coords.y * _chunkSize));
    chunk.init();
    _chunks.insert({coords, chunk});
}

bool ChunkManager::chunkExists(glm::ivec2 coords) const{
    return _chunks.find(coords) != _chunks.end();
}

void ChunkManager::render(){
    for (auto& chunk : _chunks) {
        std::cout<<"Rendering del chunk " << chunk.first.x << ", " << chunk.first.y  << std::endl;
        chunk.second.render();
    }
}

void ChunkManager::unloadDistantChunks() {
    int currentChunkX = static_cast<int>(std::floor(_cameraPosition.x / _chunkSize));
    int currentChunkZ = static_cast<int>(std::floor(_cameraPosition.z / _chunkSize));
    
    std::vector<glm::ivec2> chunksToRemove;
    
    for (auto& chunk : _chunks) {
        int chunkX = chunk.first.x;
        int chunkZ = chunk.first.y;
    
        int distanceX = std::abs(chunkX - currentChunkX);
        int distanceZ = std::abs(chunkZ - currentChunkZ);
        
        if(distanceX > _chunkVisibleInViewDistance + 1 || distanceZ > _chunkVisibleInViewDistance + 1) {
            chunksToRemove.push_back(chunk.first);
        }
    }
    
    // Remove the distant chunks
    for (const auto& coord : chunksToRemove) {
        _chunks.erase(coord);
    }
}

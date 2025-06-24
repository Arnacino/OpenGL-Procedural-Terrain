#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

class Terrain{
public:

  Terrain(std::vector<uint8_t> heightMap, glm::vec2 size, std::string textureFileName);

  void setHeightMap(std::vector<uint8_t> heightMap);

  void render(void);
  const std::vector<Vertex>& getVertices() const { return _vertices; }
  void init(void);


private:

  std::string _textureFileName;
  glm::vec2 _size;
  std::vector<uint8_t> _heightMap;
  Mesh _model;
  bool _initialized;
  std::vector<Vertex> _vertices;
  std::vector<unsigned int> _indices;
  const unsigned int NUM_STRIPS = _size.x-1;
  const unsigned int NUM_VERTS_PER_STRIP = _size.y*2;

  Terrain&operator=(const Terrain &other);
  Terrain(const Terrain &other);
  void generatePlaneMesh();
};

#endif 
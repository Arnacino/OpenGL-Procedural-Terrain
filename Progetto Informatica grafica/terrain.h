#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

class Terrain{
public:

  Terrain(std::vector<uint8_t> heightMap, glm::vec2 size, std::string textureFileName);
  void render(void);
  const std::vector<Vertex>& getVertices() const { return _vertices; }
  void init(void);


private:

  std::vector<uint8_t> _heightMap;
  std::string _textureFileName;
  glm::vec2 _size;
  Mesh _model;
  bool _initialized;
  std::vector<Vertex> _vertices;
  std::vector<unsigned int> _indices;

  Terrain&operator=(const Terrain &other);
  Terrain(const Terrain &other);
  void generatePlaneMesh();

};

#endif 
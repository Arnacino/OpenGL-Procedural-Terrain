#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"
#include "noise.h"

class Terrain{
public:

  Terrain(std::vector<float> noiseData, std::string textureFileName, std::string textureNormalFileName, int size);
  void render(void);
  const std::vector<Vertex>& getVertices() const { return _vertices; }
  void init(void);
  Terrain&operator=(const Terrain &other);
  Terrain(const Terrain &other);
  void setPosition(const glm::vec3& position) { _position = position;}
  glm::vec3 getPosition() const { return _position;}
  std::vector<float> _noiseData;

private:

  glm::vec3 _position = glm::vec3(0.0f);
  std::string _textureFileName;
  std::string _textureNormalFileName;
  int _size;
  Mesh _model;
  bool _initialized;
  std::vector<Vertex> _vertices;
  std::vector<unsigned int> _indices;

  void generatePlaneMesh();

};

#endif 
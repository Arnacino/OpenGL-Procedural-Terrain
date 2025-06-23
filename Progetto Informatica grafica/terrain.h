#ifndef FLOWER_H
#define FLOWER_H

#include "mesh.h"

class Terrain{
public:

  Terrain(float width, float depth);

  void render(void);

  void init(void);


private:

  float _width;
  float _depth;
  Mesh _model;
  bool _initialized;   
  std::vector<Vertex> _vertices;
  std::vector<unsigned int> _indices;

  Terrain&operator=(const Terrain &other);
  Terrain(const Terrain &other);
  void generatePlaneMeshData();
};

#endif 
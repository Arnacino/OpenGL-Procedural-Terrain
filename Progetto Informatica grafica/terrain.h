#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"
#include "noise.h"

class Terrain{
public:

  Terrain(std::vector<float> noiseData, std::string textureFileName, std::string textureNormalFileName, int size);
  void render(void);
  void init(void);
  Terrain&operator=(const Terrain &other);
  Terrain(const Terrain &other);

  /**
   * la position viene utilizzato come offset per posizionare nel mondo gli oggetti terreno
   */
  void setPosition(const glm::vec3& position) { _position = position;}
  
  /**
   * al posto di ricevere l'intero oggetto Noise mi salvo solo il minimo indispensabile per far funzionare il chunk, 
   * ovvero la sua dimensione (_size) e i valori di noise del chunk (_noiseData)
   */
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

  /**
   * funzione che si occupa di inizializzare i vertici della mesh (ancora piatta), che poi nel tessellation shader,
   * dopo che verranno aumentati i numeri di vertici, verra' "deformata" dalla heightMap
   */
  void generatePlaneMesh();

};

#endif 
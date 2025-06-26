/*

    Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MESH_H
#define MESH_H

#include <ostream>
#include <vector>
#include <GL/glew.h>
#include "common.h"
#include "texture2D.h"
#include "glm/glm.hpp"
#include <cstring>
#include "assimp/scene.h"       // Assimp output data structure
#include "assimp/postprocess.h" // Assimp post processing flags


/**
    Classe che incapsula la gestione dei modelli 3d caricati da file.
    La classe usa la lista dei vertici indicizzati. 
    Al momento la classe supporta modelli con una sola texture colore.
    Se il modello non ha una texture associata, viene usata una texture 
    di default "white.png"
*/
class Mesh
{
public:

    /**
        Struttura dati cpon gli attributi dei vertici
    */
    /*
    struct Vertex
    {
        glm::vec3 position; ///< Coordinate spaziali
        glm::vec3 normal;   ///< Normale
        glm::vec2 textcoord;///< Coordinate di texture

        Vertex();

        Vertex(const glm::vec3& p, const glm::vec3& n, const glm::vec2& t);
    };
*/

    Mesh();

    ~Mesh();

    /**
        Funzione che carica il modello e lo prepara per il rendering.
        
        @param filename nome del file
        @param flags assimp post-processing flags

        @return true se il modello è stato caricato correttamente
    */
    bool load_mesh(const std::string& Filename, unsigned int flags=0);

    /**
        Renderizza l'oggetto in scena usando di default
        l'id 0 per l'attributo posizione (ATTRIB_POSITIONS)
        l'id 1 per l'attributo vettori normali (ATTRIB_NORMALS)
        l'id 2 per l'attributo coordinate di texture (ATTRIB_COLOR_TEXTURE_COORDS)

        la TextureUnit 0 per la texture colore (TEXTURE_COLOR).
        la TextureUnit 1 per la texture altezza (TEXTURE_HEIGHT).
    */
    void render(void);

    /**
        Funzione che si occupa di gestire VAO, VBO E EBO per i vertici passati in input e gli indici passati in input.
    */
    bool load_mesh_from_data(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, 
    const std::string& colorTextureFileName, const std::vector<uint8_t> heightMap, const glm::vec2 size);

private:
    bool init_from_scene(const aiScene* pScene, const std::string& Filename);
    
    void clear();
    
    //funzione helper per evitare di duplicare il codice
    void setup_mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    std::string get_file_path(const std::string &Filename) const;

    unsigned int _num_indices;
    Texture2D _colorTexture;
    Texture2D _heightTexture;
    GLuint  _VAO;
    GLuint  _VBO;
    GLuint  _IBO;

    Mesh& operator=(const Mesh &other);
    Mesh(const Mesh &other);
};

std::ostream &operator<<(std::ostream &os, const Vertex &v);


#endif  /* MESH_H */


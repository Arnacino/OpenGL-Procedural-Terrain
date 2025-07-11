#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

class SkyBox{
    private:
        GLuint _skyboxVAO;
        GLuint _skyboxVBO;
        std::vector<std::string> _faces;
        unsigned int _skyboxTexture;
    public:
        void bindTexture() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxTexture);
        }
        /**
         * semplicemente carica _faces sulla texture con valore _skyboxTexture
         */
        void loadSkyboxTexture();
        void render();
        /**
         * all'interno del costruttore vengono inizializzati gia tutti i vertici con il VAO e il VBO
         */
        SkyBox(std::vector<std::string> faces);
};

#endif
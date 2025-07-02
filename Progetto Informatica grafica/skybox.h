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
        void loadSkyboxTexture();
        void render();
        SkyBox(std::vector<std::string> faces);
};

#endif
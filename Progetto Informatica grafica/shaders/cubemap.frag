#version 410 core

out vec4 FragColor;

in vec3 TexCoords;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
    FragColor = vec4(texture(skybox, TexCoords));
}
#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 Model2World;
uniform mat4 World2Camera;

out vec3 TexCoords;

void main()
{
    TexCoords = aPos;
    vec4 pos = Model2World * World2Camera * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
} 
#version 330

// Il vertex shader riceve in input gli attributi dei vertici
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 Model2World;

// Questa variabile serve per passare in output l'informazione colore agli altri 
// shader. In particolare ci serve nel Fragment Shader
flat out vec3 vertex_color;

void main()
{
    gl_Position = Model2World * vec4(position, 1.0);
    vertex_color = color; 
}
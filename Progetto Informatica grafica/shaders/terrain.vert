#version 400

// Il vertex shader riceve in input gli attributi dei vertici
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textcoord;  


//da passare al tessellation shader
out vec2 TexCoord;


void main()
{
    //convertito in coordinate omogenee e basta
    gl_Position = vec4(position, 1.0);

    TexCoord = textcoord;
}
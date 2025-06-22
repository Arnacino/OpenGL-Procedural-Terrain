#version 330

// Questa variabile di input ha lo stesso nome di quella nel Vertex Shader
// E' importante che i nomi siano uguali perchè solo in questo modo si ha
// il passaggio delle informazioni.
flat in vec3 vertex_color;

out vec4 color;

void main()
{
	// Ci limitiamo a passare il colore di input in output.
    // Dato che il colore di input è un vec3 e a noi serve un vec4, ne creiamo
    // uno al volo da quello di input aggiungendo la quarta componente.
    color = vec4(vertex_color, 1.0);
}
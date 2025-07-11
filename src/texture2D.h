#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

/**
	Classe di supporto per caricare in memoria una texture da un file o da una heightmap passata in input

	Supporta solo immagini PNG. 
*/
class Texture2D {
public:
	/**
		Costruttore
	*/
	Texture2D();

	/**
		Carica in memoria una texture di tipo TextureTarget presa da
		file.

		@param TextureTarget tipo di texture (es. GL_TEXTURE_2D)
		@param FileName nome del file 

		@throw una eccezione ivllib::ImageFormatError in caso di errore
	*/
	bool load(const std::string& FileName);

	//overload del metodo load per caricare una heightMap come texture direttamente
	bool load(const std::vector<float>& heightMap, int size);

	/**
		Attiva la textureUnit indicata e binda la texture ad essa.
	*/
	void bind(int TextureUnit) const;

	/**
		Controllo di validità della texture.
		@return true se la texture è stata caricata correttamente.
	*/
	bool is_valid(void) const;

private:
    std::string _filename; ///<< Nome del file
    GLuint _texture; ///<< Oggetto OpenGL che rappresenta la texture
    bool _valid; ///<< Flag di validità
};

#endif
#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

/**
	Classe che incapsula diverse funzioni per calcolare le matrici di 
	trasformazione di vista e prospettica. 

*/
class Camera {

public:
	/**
		Costruttore. Inizializza le matrici all'identità.
	*/
	Camera();

	/**
		Imposta la matrice di trasformazione di vista.

		@param position posizione della camera
		@param target punto dove guarda la camera
		@param up vettore che indica l'alto della camera
	*/
	void set_camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up);

	/**
		Ritorna la matrice di trasformazione di vista.

		@return la matrice di trasformazione di vista.
	*/
	const glm::mat4& view() const;
	
	/**
		Genera la matrice di trasformazione di camera.

		@param position posizione della camera
		@param target punto dove guarda la camera
		@param up vettore che indica l'alto della camera

		@return la matrice di trasformazione di camera	
	*/
	static glm::mat4 camera_setting(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up);

	/**
		Imposta la matrice di trasformazione di proiezione prospettica.

		@param FOVDeg Angolo del field of view in gradi 
		@param width larghezza della window
		@param height altezza della window
		@param znear coordinata z del near plane
		@param zfar coordinate z del far plane
	*/
	void set_perspective(float FOVDeg, float width, float height, float znear, float zfar);

	/**
		Ritorna la matrice di trasformazione prospettica.
		@return la matrice di trasformazione prospettica
	*/
	const glm::mat4& projection() const;

	/**
		Ritorna la matrice di trasformazione di proiezione prospettica.

		@param FOVDeg Angolo del field of view in gradi 
		@param width larghezza della window
		@param height altezza della window
		@param znear coordinata z del near plane
		@param zfar coordinate z del far plane
		@return la matrice di trasformazione di proiezione prospettica
	*/
	glm::mat4 perspective_projection(float FOVDeg, float width, float height, float znear, float zfar);


	/**
		Reimposta le matrici di trasformazione all'identità
	*/
	void reset();


	/**
		Ritorna la matrice di trasformazione completa. 
		@return la matrice di trasformazione completa.
	*/
	const glm::mat4& CP() const;

private:
	glm::mat4 _view;     ///<< matrice di trasformazione di vista

	glm::mat4 _projection; ///<< matrice di trasformazione di proiezione 

	glm::mat4 _combined; ///<< matrice di trasformazione completa

	void update();
};


#endif
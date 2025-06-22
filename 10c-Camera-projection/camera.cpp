#include "camera.h"

#define  GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() {
	reset();
}

void Camera::reset() {
	_combined = _projection = _view = glm::mat4(1.0f);
}

void Camera::update() {
	_combined = _projection * _view;  
}

const glm::mat4& Camera::CP() const {
	return _combined;
}

void Camera::set_camera(const glm::vec3 &position, const glm::vec3 &lookat, const glm::vec3 &up){
	_view = camera_setting(position,lookat,up);
	update();
}

glm::mat4 Camera::camera_setting(const glm::vec3 &position, const glm::vec3 &lookat, const glm::vec3 &up) {

	glm::mat4 V = glm::lookAt(position,lookat,up);

	return V;
}

const glm::mat4& Camera::view() const {
	return _view;
}

void Camera::set_perspective(float FOVDeg, float width, float height, float znear, float zfar) {
	_projection = perspective_projection(FOVDeg,width,height,znear,zfar);
	update();
}

glm::mat4 Camera::perspective_projection(float FOVDeg, float width, float height, float znear, float zfar) {
	assert(zfar > znear);
	assert(width>0);
	assert(height>0);

  glm::mat4 p = glm::perspective(
    	glm::radians(FOVDeg),
    	width/height,
    	znear, 
    	zfar);

	return p;
}

const glm::mat4& Camera::projection() const {
	return _projection;
}

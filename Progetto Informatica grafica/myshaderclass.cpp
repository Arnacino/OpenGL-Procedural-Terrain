#include "myshaderclass.h"
#include "utilities.h"
#include "common.h"
#include <iostream>

void MyShaderClass::set_model_transform(const glm::mat4 &transform) {
  glUniformMatrix4fv(_model_transform_location, 1, GL_FALSE, const_cast<float *>(&transform[0][0]));       
}

void MyShaderClass::set_camera_transform(const glm::mat4 &transform) {
  glUniformMatrix4fv(_camera_transform_location, 1, GL_FALSE, const_cast<float *>(&transform[0][0]));       
}

void MyShaderClass::set_ambient_light(const AmbientLight &al) {
  glUniform3fv(_ambient_color_location, 1, const_cast<float *>(&al.color()[0]));
  glUniform1f(_ambient_intensity_location, al.intensity());
}

void MyShaderClass::set_diffusive_light(const DiffusiveLight &dl) {
  glUniform1f(_diffusive_intensity_location, dl.intensity());
}

void MyShaderClass::set_directional_light(const DirectionalLight &dl) {
  glm::vec3 direction_normalized = glm::normalize(dl.direction());
  glUniform3fv(_directional_color_location, 1, const_cast<float *>(&dl.color()[0]));
  glUniform3fv(_directional_direction_location, 1, const_cast<float *>(&direction_normalized[0]));
}

void MyShaderClass::set_specular_light(const SpecularLight &sl) {
  glUniform1f(_specular_intensity_location, sl.intensity());
  glUniform1f(_specular_shininess_location, sl.shininess());
}

void MyShaderClass::set_camera_position(const glm::vec3 &pos) {
  glUniform3fv(_camera_position_location, 1, const_cast<float *>(&pos[0]));
}

void MyShaderClass::set_color_texture(){
  glUniform1i(_colorTexture_location, TEXTURE_COLOR);
}

void MyShaderClass::set_height_texture(){
  glUniform1i(_heightmap_location, TEXTURE_HEIGHT);
}

void MyShaderClass::set_color_normal_texture(){
  glUniform1i(_colorTextureNormal_location, TEXTURE_COLOR_NORMAL);
}

void MyShaderClass::set_fog_end(float end) {
    glUniform1f(_fog_end_location, end);
}

bool MyShaderClass::load_shaders() {
    switch(_shaderType) {
        case TERRAIN:
            return load_terrain_shaders();
        case CUBEMAP:
            return load_cubemap_shaders();
        default:
            return false;
    }
}

bool MyShaderClass::load_terrain_shaders() {
    return  add_shader(GL_VERTEX_SHADER,"shaders/terrain.vert") &&
            add_shader(GL_FRAGMENT_SHADER,"shaders/terrain.frag") &&
            add_shader(GL_TESS_CONTROL_SHADER, "shaders/terrain.tesc") &&
            add_shader(GL_TESS_EVALUATION_SHADER, "shaders/terrain.tese");
}

bool MyShaderClass::load_cubemap_shaders() {
    return  add_shader(GL_VERTEX_SHADER,"shaders/cubemap.vert") &&
            add_shader(GL_FRAGMENT_SHADER,"shaders/cubemap.frag");
}

bool MyShaderClass::load_done() {
    _model_transform_location = get_uniform_location("Model2World");
    _camera_transform_location = get_uniform_location("World2Camera");

    if (_shaderType == TERRAIN) {
        _camera_position_location = get_uniform_location("CameraPosition");
        _ambient_color_location = get_uniform_location("AmbientLight.color");
        _ambient_intensity_location = get_uniform_location("AmbientLight.intensity");
        _directional_color_location = get_uniform_location("DirectionalLight.color");
        _directional_direction_location = get_uniform_location("DirectionalLight.direction");
        _diffusive_intensity_location = get_uniform_location("DiffusiveLight.intensity");
        _specular_intensity_location = get_uniform_location("SpecularLight.intensity");
        _specular_shininess_location = get_uniform_location("SpecularLight.shininess");
        _colorTexture_location = get_uniform_location("colorTexture");
        _heightmap_location = get_uniform_location("heightMap");
        _colorTextureNormal_location = get_uniform_location("colorTextureNormal");
        _fog_end_location = get_uniform_location("fogEnd");

        return (_model_transform_location != INVALID_UNIFORM_LOCATION) &&
              (_camera_transform_location != INVALID_UNIFORM_LOCATION) &&
              (_ambient_color_location != INVALID_UNIFORM_LOCATION) &&
              (_ambient_intensity_location != INVALID_UNIFORM_LOCATION) &&
              (_directional_color_location != INVALID_UNIFORM_LOCATION) &&
              (_directional_direction_location != INVALID_UNIFORM_LOCATION) &&
              (_diffusive_intensity_location != INVALID_UNIFORM_LOCATION) &&
              (_specular_intensity_location != INVALID_UNIFORM_LOCATION) &&
              (_specular_shininess_location != INVALID_UNIFORM_LOCATION) &&
              (_camera_position_location != INVALID_UNIFORM_LOCATION) &&
              (_colorTexture_location != INVALID_UNIFORM_LOCATION) && 
              (_heightmap_location != INVALID_UNIFORM_LOCATION) &&
              (_colorTextureNormal_location != INVALID_UNIFORM_LOCATION) &&
              (_fog_end_location != INVALID_UNIFORM_LOCATION);
    } else {
        // Per il cubemap shader ci servono solo queste uniform
        return (_model_transform_location != INVALID_UNIFORM_LOCATION) &&
              (_camera_transform_location != INVALID_UNIFORM_LOCATION);
      }
}

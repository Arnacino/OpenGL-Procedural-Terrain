#ifndef MYSHADERCLASS_H
#define MYSHADERCLASS_H

#include "shaderclass.h"
#include "light.h"

/**
    Classe che include le funzionalità specifiche legate agli shader da usare
    nell'applicazione. 
    Sono stati overloadati i metodi load_shaders e load_done.
    Sono stati inseriti due metodi pubblici per settare la matrice di trasformazione
    delle coordinate dei vertici. 
*/
class MyShaderClass : public ShaderClass {
public:

    enum ShaderType {
        TERRAIN,
        CUBEMAP
    };

    void setShaderType(ShaderType type) { _shaderType = type; }

    /**
        Setta la matrice di trasformazione nel vertex shader

        @param transform matrice 4x4 di trasformazione  
    */
    void set_model_transform(const glm::mat4 &transform);

    /**
        Setta la matrice di trasformazione di camera completa

        @param transform matrice 4x4 di trasformazione  
    */
    void set_camera_transform(const glm::mat4 &transform);

    /**
        Setta le proprietà della luce ambientale

        @param al informazioni relative alla luce ambientale
    */
    void set_ambient_light(const AmbientLight &al);

    /**
        Setta le proprietà della luce diffusiva

        @param dl informazioni relative alla luce diffusiva
    */
    void set_diffusive_light(const DiffusiveLight &dl);

    /**
        Setta le proprietà della luce direzionale

        @param dl informazioni relative alla luce direzionale
    */
    void set_directional_light(const DirectionalLight &dl);

    /**
        Setta le proprietà della luce speculare

        @param sl informazioni relative alla luce speculare
    */
    void set_specular_light(const SpecularLight &sl);

    void set_camera_position(const glm::vec3 &pos);


    /**
     * questi metodi sono stati aggiunti per far si di poter usare piu' texture units alla volta.
     * per come era gestita prima shaderClass si poteva usare solo il textureUnit0
     */
    void set_color_texture();

    void set_height_texture();

    void set_color_normal_texture();

    void set_fog_end(float end);



private:


    /**
     * questi metodi servono per caricare correttamente gli shader senza creare due classi separate ma usando 
     * un enum per identificare quale caricare. L'enum e' dicharato nella classe ShaderClass
     */
    bool load_terrain_shaders();
    bool load_cubemap_shaders();
    
    ShaderType _shaderType = TERRAIN;

    /**
        Metodo per il caricamento degli shader

        @return true se il caricamento è andato a buon fine
    */
    virtual bool load_shaders();

    /**
        Metodo che recupera tutte le location delle variabili uniform 
        negli shader caricati.  

        @return se le operazioni post-load sono andate a buon fine
    */
    virtual bool load_done();

    GLint _model_transform_location; ///<< Location della variabile Model2World
    GLint _camera_transform_location; ///<< Location della variabile World2Camera

    GLint _ambient_color_location; ///<< Location del colore della luce ambientale
    GLint _ambient_intensity_location; ///<< Location dell'intensità della luce ambientale

    GLint _directional_color_location;     ///<< Location del colore della luce direzionale
    GLint _directional_direction_location; ///<< Location della direzione della luce direzionale
    GLint _diffusive_intensity_location;   ///<< Location dell'intensità della luce diffusiva

    GLint _specular_intensity_location;
    GLint _specular_shininess_location;
    GLint _camera_position_location;
    
    GLint _heightmap_location;
    GLint _colorTexture_location;
    GLint _colorTextureNormal_location;

    GLuint _fog_end_location;
};
#endif
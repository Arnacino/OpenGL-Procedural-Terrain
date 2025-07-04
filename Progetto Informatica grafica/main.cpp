/**
  Questo codice mostra come sfruttare la libreria esterna assimp per importare dei
  modelli 3D e renderizzarli nella scena.
  Al momento, il codice è in grado di renderizzare modelli costituiti da una
  singola mesh e con una singola Texture associata.
*/


#include <iostream>
#include <sstream>
#include <vector>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "transform.h"
#include "camera.h"
#include "noise.h"
#include "myshaderclass.h"

#include "chunkManager.h"
#include "skybox.h"

GLint MODE = GL_FILL;

/*
  Struttura di comodo dove sono memorizzate tutte le variabili globali
*/
struct global_struct {

  int WINDOW_WIDTH  = 1920; // Larghezza della finestra 
  int WINDOW_HEIGHT = 1080; // Altezza della finestra

  glm::vec2 noiseSize = glm::vec2(200,200);
  float noiseScale = 180.0f;
  int noiseOctaves = 5;
  float noisePersistance = 0.3f;
  float noiseLacunarity = 2.0f;
  float noiseSeed = 4.0f;
  glm::vec2 noiseOffset = {0,0};

  glm::vec3 initialCameraPos = {0,80,80};

  Camera camera;
  Noise noise;

  AmbientLight     ambient_light;
  DirectionalLight directional_light;
  DiffusiveLight   diffusive_light;
  SpecularLight    specular_light;

  MyShaderClass myshaders;
  MyShaderClass cubeMapShader;

  const float SPEED = 1;

  float gradX;
  float gradY; 

  global_struct() : gradX(0.0f), gradY(0.0f), noise(
    noiseSize, noiseScale, noiseOctaves, noisePersistance, 
    noiseLacunarity, noiseSeed, noiseOffset) {}

} global;

ChunkManager* chunkManager = nullptr;
SkyBox* skybox = nullptr;

/**
Prototipi della nostre funzioni di callback. 
Sono definite più avanti nel codice.
*/
void MyRenderScene(void);
void MyIdle(void);
void MyKeyboard(unsigned char key, int x, int y);
void MyClose(void);
void MySpecialKeyboard(int Key, int x, int y);
void MyMouse(int x, int y);

void init(int argc, char*argv[]) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);


  glutInitWindowSize(global.WINDOW_WIDTH, global.WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Informatica Grafica");


/*   // Imposta la modalità a schemo intero e nasconde il cursore
  std::stringstream game_mode;
  game_mode << global.WINDOW_WIDTH << "x" << global.WINDOW_HEIGHT << ":32";
  glutGameModeString(game_mode.str().c_str());
  glutEnterGameMode();
 */
  glutSetCursor(GLUT_CURSOR_NONE);

  global.camera.set_mouse_init_position(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);
  global.camera.lock_mouse_position(true);
  glutWarpPointer(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);


  // Must be done after glut is initialized!
  GLenum res = glewInit();
  if (res != GLEW_OK) {
      std::cerr<<"Error : "<<glewGetErrorString(res)<<std::endl;
    exit(1);
  }

  // Inizializza qui gli oggetti che richiedono OpenGL
  skybox = new SkyBox(std::vector<std::string>{
        "assets/skybox/right.jpg",  //RIGHT
        "assets/skybox/left.jpg",  //LEFT
        "assets/skybox/top.jpg",  //TOP
        "assets/skybox/bot.jpg",  //BOT
        "assets/skybox/back.jpg",  //BACK
        "assets/skybox/front.jpg"   //FRONT
  });
  chunkManager = new ChunkManager(1000.0f, global.initialCameraPos, 
              global.noise, "assets/textures/sandstone.jpg", "assets/textures/sandstone_normal.jpg");

  glClearColor(0.239f, 0.239f, 0.38f, 1.0f);
  
  glEnable(GL_FRAMEBUFFER_SRGB);
  
  glPatchParameteri(GL_PATCH_VERTICES, 4);

  glutDisplayFunc(MyRenderScene);

  glutKeyboardFunc(MyKeyboard);

  glutCloseFunc(MyClose);

  glutPassiveMotionFunc(MyMouse);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_DEPTH_TEST);

}

void create_scene() {

  global.camera.set_camera(
          global.initialCameraPos,
          glm::vec3(0, 0, 0),
          glm::vec3(0, 1, 0)
      );

  global.camera.set_perspective(
    60.0f,
    global.WINDOW_WIDTH,
    global.WINDOW_HEIGHT,
    0.1,
    10000
  );

  global.myshaders.setShaderType(MyShaderClass::TERRAIN);
  global.cubeMapShader.setShaderType(MyShaderClass::CUBEMAP);
  global.myshaders.init();
  global.cubeMapShader.init();

  global.ambient_light = AmbientLight(glm::vec3(1,1,1), 0.21); 
  global.directional_light = DirectionalLight(glm::vec3(1,1,1), glm::vec3(0,-1,0));
  global.diffusive_light = DiffusiveLight(0.2); 
  global.specular_light = SpecularLight(1,0.5);
  
  skybox->loadSkyboxTexture();

}



void Render_terrain(){
  LocalTransform modelT;

  global.myshaders.set_model_transform(modelT.T());
  // Update chunks based on camera position
  chunkManager->update(global.camera.position());
    
  // Render all visible chunks
  chunkManager->render();
}

void Render_cubemap(){
    LocalTransform modelT;
    global.cubeMapShader.set_model_transform(modelT.T());
    glm::mat4 view = glm::mat4(glm::mat3(global.camera.camera()));
    glm::mat4 projView = global.camera.projection() * view;
    global.cubeMapShader.set_camera_transform(projView);
    skybox->render();
}

void MyRenderScene() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  global.myshaders.enable();
  glActiveTexture(GL_TEXTURE0 + TEXTURE_COLOR);
  global.myshaders.set_color_texture();
  glActiveTexture(GL_TEXTURE0 + TEXTURE_HEIGHT);
  global.myshaders.set_height_texture();
  global.myshaders.set_camera_transform(global.camera.CP());
  global.myshaders.set_ambient_light(global.ambient_light);
  global.myshaders.set_directional_light(global.directional_light);
  global.myshaders.set_diffusive_light(global.diffusive_light);
  global.myshaders.set_specular_light(global.specular_light);
  //global.myshaders.set_camera_position(global.camera.position());
  global.myshaders.set_color_texture();
  global.myshaders.set_height_texture();
  Render_terrain();

  // Poi renderizza la skybox
  glDepthFunc(GL_LEQUAL);  // Cambia depth function
  global.cubeMapShader.enable();
  Render_cubemap();
  glDepthFunc(GL_LESS);    // Ripristina depth function default
  
  glutSwapBuffers();
}

// Funzione globale che si occupa di gestire l'input da tastiera.
void MyKeyboard(unsigned char key, int x, int y) {
  switch ( key )
  {
    case 27: // Escape key
      glutDestroyWindow(glutGetWindow());
      return;
    break;

    case ']': 
        global.camera.set_speed(global.camera.speed()*1.1);
        break;

    case '[': 
        global.camera.set_speed(global.camera.speed()/1.1);
        break;

    // comandi di movimento
    case 'w':
    case 'a':
    case 'd':
    case 's':
      global.camera.onKeyboard(key);
      break;

    // Variamo l'intensità di luce ambientale
    case '1':
      global.ambient_light.dec(0.01);
    break;

    // Variamo l'intensità di luce ambientale
    case '2':
      global.ambient_light.inc(0.01);
    break;

    // Variamo l'intensità di luce diffusiva
    case '3':
      global.diffusive_light.dec(0.01);
    break;

    // Variamo l'intensità di luce diffusiva
    case '4':
      global.diffusive_light.inc(0.01);
    break;

    // Variamo l'intensità di luce speculare
    case '5':
      global.specular_light.dec(0.01);
    break;

    // Variamo l'intensità di luce speculare
    case '6':
      global.specular_light.inc(0.01);
    break;

    // Variamo l'esponente della luce speculare
    case '7':
      global.specular_light.dec_shine(1);
    break;

    // Variamo l'esponente della luce speculare
    case '8':
      global.specular_light.inc_shine(1);
    break;

  case 'l': // Move light left
      global.directional_light = DirectionalLight(glm::vec3(1,1,1), 
          glm::normalize(global.directional_light.direction() + glm::vec3(-0.1f, 0, 0)));
      break;

  case 'j': // Move light right
      global.directional_light = DirectionalLight(glm::vec3(1,1,1), 
          glm::normalize(global.directional_light.direction() + glm::vec3(0.1f, 0, 0)));
      break;

  case 'i': // Move light up
      global.directional_light = DirectionalLight(glm::vec3(1,1,1), 
          glm::normalize(global.directional_light.direction() + glm::vec3(0, -0.1f, 0)));
      break;

  case 'k': // Move light down
      global.directional_light = DirectionalLight(glm::vec3(1,1,1), 
          glm::normalize(global.directional_light.direction() + glm::vec3(0, 0.1f, 0)));
      break;

  case 'o': // Move light forward
      global.directional_light = DirectionalLight(glm::vec3(1,1,1), 
          glm::normalize(global.directional_light.direction() + glm::vec3(0, 0, -0.1f)));
      break;

  case 'u': // Move light backward
      global.directional_light = DirectionalLight(glm::vec3(1,1,1), 
          glm::normalize(global.directional_light.direction() + glm::vec3(0, 0, 0.1f)));
      break;

    case ' ': // Reimpostiamo la camera
      global.camera.set_camera(
          global.initialCameraPos,
          glm::vec3(0, 0, -1),
          glm::vec3(0, 1, 0)
      );
      global.gradX = 0;
      global.gradY = 0;

    break;

    case 'z':
      if(MODE == GL_FILL) {
        MODE = GL_LINE;
        glDisable(GL_CULL_FACE);
      }
      else {
        MODE = GL_FILL;
        glEnable(GL_CULL_FACE);
      }
      glPolygonMode(GL_FRONT_AND_BACK,MODE);
    break;
  }

  glutPostRedisplay();
}


void MyMouse(int x, int y) {
  if (global.camera.onMouse(x,y)) {
    // Risposto il mouse al centro della finestra
    glutWarpPointer(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);
  }
  glutPostRedisplay();
}


// Funzione globale che si occupa di gestire la chiusura della finestra.
void MyClose(void) {
  std::cout << "Tearing down the system..." << std::endl;
  // A schermo intero dobbiamo uccidere l'applicazione.
  exit(0);
}

int main(int argc, char* argv[])
{
  init(argc,argv);
  create_scene();

  glutMainLoop();
  
  return 0;
}
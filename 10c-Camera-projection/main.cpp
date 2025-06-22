/**
    Questo codice mostra come si imposta la trasformazione di camera.

    Camera: è una classe che permette di calcolare la trasformazione di 
    camera e prospettica.

*/


#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h"
#include "transform.h"
#include "camera.h"
/**
    Struttura di comodo dove sono memorizzate tutte le variabili globali
*/
struct global_struct {

    int WINDOW_WIDTH = 1024; // Larghezza della finestra 
    int WINDOW_HEIGHT = 768; // Altezza della finestra

    GLuint VAO; // Vertex Array Object

    GLuint program; // id del programma degli shader

    Camera camera; // Trasformazione di camera

} global;

/**
    Struttura dati che contiene gli attributi di un vertice.
*/
struct Vertex {
    glm::vec3 position; ///< Coordinate spaziali
    glm::vec3 color;    ///< Colore
    Vertex(float x, float y, float z, float r, float g, float b) {
        position = glm::vec3(x,y,z);
        color = glm::vec3(r,g,b);
    }

    Vertex(const glm::vec3 &xyz, const glm::vec3 &rgb) : position(xyz), color(rgb) {}
};

/**
  Prototipi della nostre funzioni di callback. 
  Sono definite più avanti nel codice.
*/
void MyRenderScene(void);
void MyKeyboard(unsigned char key, int x, int y);
void MyClose(void);

void init(int argc, char*argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Informatica Grafica");


   // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cerr<<"Error : "<<glewGetErrorString(res)<<std::endl;
      exit(1);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glutDisplayFunc(MyRenderScene);

    glutKeyboardFunc(MyKeyboard);

    glutCloseFunc(MyClose);


    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);

    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);

}

void load_shader_program() {
    
    Shaders s;

    s.push_back(LoadShader(GL_VERTEX_SHADER,"10.vert"));
    s.push_back(LoadShader(GL_FRAGMENT_SHADER,"10.frag"));

    global.program = CreateProgram(s); 
}

void create_scene() {

    Vertex Vertices[4] = {
        Vertex(glm::vec3(-1.0f, -1.0f,-1.0f), glm::vec3( 1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3( 0.0f, 1.0f, 0.0f)),
        Vertex(glm::vec3( 0.0f,  1.0f,-1.0f), glm::vec3( 0.0f, 0.0f, 1.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f,-1.0f), glm::vec3( 1.0f, 1.0f, 0.0f))
    };

    unsigned int Indices[] = {
        1, 2, 0,
        3, 2, 1,
        3, 0, 2,
        1, 0, 3        
    };

    glGenVertexArrays(1, &(global.VAO));
    glBindVertexArray(global.VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        reinterpret_cast<GLvoid*>(offsetof(struct Vertex, position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
         reinterpret_cast<GLvoid*>(offsetof(struct Vertex, color)));
    glEnableVertexAttribArray(1);


    GLuint IBO; // Index Buffer Object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // Impostiamo la trasformazione di camera
    // 
    // Usiamo l'oggetto globale camera.
    // Il primo parametro è la posizione della camera nella scena
    // Il secondo parametro è il punto look-at cioè dove sta guardando la camera 
    // Il terzo parametro è il vettore VUP
    //
    // Provate a modificare i parametri e vedere cosa succede. 
    global.camera.set_camera(
        glm::vec3(0,0,4),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );

    // Impostiamo la trasformazione prospettica
    // 
    // Usiamo l'oggetto globale camera.
    // Il primo parametro è il Field Of View in gradi
    // Il secondo e terzo parametro è la dimensione della finestra di output 
    // Il quarto parametro è la posizione z del near plane
    // Il quinto parametro è la posizione z del far plane
    //
    // Tutto ciò che ha coordinate z fuori dai range impostati, 
    // non viene visualizzato, è clippato.
    global.camera.set_perspective(
        45.0f,
        global.WINDOW_WIDTH,
        global.WINDOW_HEIGHT,
        0.1,
        100);
}

void MyRenderScene() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(global.program);

    GLuint transformLocation = glGetUniformLocation(global.program,"Model2World");
 
    static float theta = 0;
    theta += 1; // Incrementiamo l'angolo di un grado
    if (theta>360) theta = 0; // wrap-around

    LocalTransform modelT;
    modelT.rotate(0,theta,0); // Ruotiamo di theta (gradi)

    // Creiamo la trasformazione completa moltiplicando la trasformazione di 
    // camera completa per la matrice composita del modello
    glm::mat4 M = global.camera.CP() * modelT.T();

    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &M[0][0]);

    glBindVertexArray(global.VAO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glutSwapBuffers();

    glutPostRedisplay();
}

// Funzione globale che si occupa di gestire l'input da tastiera.
void MyKeyboard(unsigned char key, int x, int y) {
    switch ( key )
    {
        case 27: // Escape key
        glutDestroyWindow(glutGetWindow());
        break;
    }
}

// Funzione globale che si occupa di gestire la chiusura della finestra.
void MyClose(void) {
    std::cout << "Tearing down the system..." << std::endl;
}

int main(int argc, char* argv[])
{
    init(argc,argv);

    load_shader_program();

    create_scene();

    glutMainLoop();
    
    return 0;
}
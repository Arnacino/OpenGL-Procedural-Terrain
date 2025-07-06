// tessellation control shader
#version 410 core

//dimensioni di input e output
layout (vertices=4) out;

// coordinate di texture in input
in vec2 TexCoord[];
// coordinate di texture in output
out vec2 TextureCoord[];

uniform mat4 Model2World;
uniform mat4 World2Camera;

const int NUM_LOD_LEVELS = 4;
const float LOD_DISTANCES[NUM_LOD_LEVELS] = float[](300.0, 350.0, 450.0, 550.0);
const int LOD_TESS_LEVELS[NUM_LOD_LEVELS] = int[](24, 16, 8, 4);
const int MIN_TESS_LEVEL = 2;


// Funzione per determinare il livello di tessellazione basato sulla distanza
//essenzialmente tra 0-300 max, tra 300-350 16, 350-450 8, 450-550 4, >550 2
int getLODLevel(float distance) {
    for(int i = 0; i < NUM_LOD_LEVELS; i++) {
        if(distance < LOD_DISTANCES[i]) {
            return LOD_TESS_LEVELS[i];
        }
    }
    return MIN_TESS_LEVEL;
}

void main()
{

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];

    //assicura che i livelli di tessellazione vengano calcolati solo una volta
    if (gl_InvocationID == 0)
    {

        //posizione di ciascun vertice della patch rispetto alle coordinate di camera
        vec4 eyeSpacePos00 = World2Camera * Model2World * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = World2Camera * Model2World * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = World2Camera * Model2World * gl_in[2].gl_Position;
        vec4 eyeSpacePos11 = World2Camera * Model2World * gl_in[3].gl_Position;

        // Calcola il livello di tessellazione per ogni vertice
        float tess00 = float(getLODLevel(abs(eyeSpacePos00.z)));
        float tess01 = float(getLODLevel(abs(eyeSpacePos01.z)));
        float tess10 = float(getLODLevel(abs(eyeSpacePos10.z)));
        float tess11 = float(getLODLevel(abs(eyeSpacePos11.z)));

        // Usa il livello di tessellazione più alto per ogni edge per evitare cracks
        gl_TessLevelOuter[0] = max(tess10, tess00);
        gl_TessLevelOuter[1] = max(tess00, tess01);
        gl_TessLevelOuter[2] = max(tess01, tess11);
        gl_TessLevelOuter[3] = max(tess11, tess10);

        // Usa il livello più alto per l'interno
        gl_TessLevelInner[0] = max(gl_TessLevelOuter[1], gl_TessLevelOuter[3]);
        gl_TessLevelInner[1] = max(gl_TessLevelOuter[0], gl_TessLevelOuter[2]);
    }
}


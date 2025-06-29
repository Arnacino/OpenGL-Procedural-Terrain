// tessellation control shader
#version 410 core

// specify number of control points per patch output
// this value controls the size of the input and output arrays
layout (vertices=4) out;

// coordinate di texture in input
in vec2 TexCoord[];
// coordinate di texture in output
out vec2 TextureCoord[];

uniform mat4 Model2World;
uniform mat4 World2Camera;

void main()
{

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];


    if (gl_InvocationID == 0)
    {
        const int MIN_TESS_LEVEL = 4;
        const int MAX_TESS_LEVEL = 64;
        const float MIN_DISTANCE = 50;
        const float MAX_DISTANCE = 200;
        
        
        //posizione di ciascun vertice della patch rispetto alle coordinate di camera
        vec4 eyeSpacePos00 = World2Camera * Model2World * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = World2Camera * Model2World * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = World2Camera * Model2World * gl_in[2].gl_Position;
        vec4 eyeSpacePos11 = World2Camera * Model2World * gl_in[3].gl_Position;

        //calcolo della distanza effettiva
        float distance00 = clamp((abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance01 = clamp((abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance10 = clamp((abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance11 = clamp((abs(eyeSpacePos11.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        // ----------------------------------------------------------------------
        // Step 4: interpolate edge tessellation level based on closer vertex
        float tessLevel0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
        float tessLevel1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
        float tessLevel2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
        float tessLevel3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );

        // ----------------------------------------------------------------------
        // Step 5: set the corresponding outer edge tessellation levels
        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        // ----------------------------------------------------------------------
        // Step 6: set the inner tessellation levels to the max of the two parallel edges
        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}
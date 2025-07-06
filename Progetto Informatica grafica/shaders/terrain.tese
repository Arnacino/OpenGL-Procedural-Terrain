// tessellation evaluation shader
#version 410 core

layout (quads, fractional_odd_spacing, ccw) in;

uniform sampler2D heightMap;
uniform mat4 Model2World;
uniform mat4 World2Camera;
uniform sampler2D colorTextureNormal;

// coordinate di texture ricevute dalla tassellazione
in vec2 TextureCoord[];

//coordinate della texture da passare al fragment
out vec2 texCoord;

//normali calcolate dopo il displacement
out vec4 tess_normal;

//posizione dei vertici dopo il displacement
out vec3 position;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];

    // Interpolazione bilineare per coordinate UV
    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    texCoord = (t1 - t0) * v + t0;

    //calcolo dell'altezza dalla texture
    float Height = texture(heightMap, texCoord).r * 64;

    
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // Interpolazione bilienare per posizione
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    float texelSize = 1.0/textureSize(heightMap, 0).x;
    float step = 2.0 * texelSize;
    
    float heightL = texture(heightMap, texCoord - vec2(step, 0.0)).r * 32;
    float heightR = texture(heightMap, texCoord + vec2(step, 0.0)).r * 32;
    float heightD = texture(heightMap, texCoord - vec2(0.0, step)).r * 32;
    float heightU = texture(heightMap, texCoord + vec2(0.0, step)).r * 32;
    
    // Calcolo delle tangenti usando le differenze di altezza
    float scale = 1.0;
    vec3 tangentX = normalize(vec3(scale, heightR - heightL, 0.0));
    vec3 tangentZ = normalize(vec3(0.0, heightU - heightD, scale));

    vec3 normal = normalize(cross(tangentZ, tangentX));

    vec3 normalFromMap = texture(colorTextureNormal, texCoord).rgb * 2.0 - 1.0;
    vec3 T = tangentX;
    vec3 N = normal;
    vec3 B = normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    vec3 finalNormal = normalize(TBN * normalFromMap);
    tess_normal = vec4(normalize(mat3(Model2World) * finalNormal), 0.0);

    // modifico l'altezza tramite heightmap dopo aver modificato le normali
    p.y += Height;

    //posizione in clip space
    gl_Position = World2Camera * Model2World * p;

    vec4 worldPos = Model2World * p;
    vec3 worldNormal = mat3(Model2World) * tess_normal.xyz;
    tess_normal = vec4(normalize(worldNormal), 0.0);

    position = worldPos.xyz;
}
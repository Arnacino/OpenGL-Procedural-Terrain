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

    //interpolazione texdture coords
    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];
    vec2 t0 = mix(t00, t01, u);
    vec2 t1 = mix(t10, t11, u);
    texCoord = mix(t0, t1, v);

    //calcolo dell'altezza dalla texture
    float heightScale = 64;
    float Height = texture(heightMap, texCoord).r * heightScale;

    // Interpolazione posizione
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;
    vec4 p0 = mix(p00, p01, u);
    vec4 p1 = mix(p10, p11, u);
    vec4 p = mix(p0, p1, v);

    p.y += Height;

    // === Calcolo normali da heightmap ===
    float texelSize = 1.0 / textureSize(heightMap, 0).x;
    float offset = 2.0 * texelSize;

    float heightL = texture(heightMap, texCoord - vec2(offset, 0.0)).r * heightScale;
    float heightR = texture(heightMap, texCoord + vec2(offset, 0.0)).r * heightScale;
    float heightD = texture(heightMap, texCoord - vec2(0.0, offset)).r * heightScale;
    float heightU = texture(heightMap, texCoord + vec2(0.0, offset)).r * heightScale;

    float scale = 2.0; // distanza nel piano xz
    vec3 tangentX = normalize(vec3(2.0 * scale, heightR - heightL, 0.0));
    vec3 tangentZ = normalize(vec3(0.0, heightU - heightD, 2.0 * scale));
    vec3 normal = normalize(cross(tangentZ, tangentX));

    // === Bump map ===
    vec3 normalFromMap = texture(colorTextureNormal, texCoord).rgb * 2.0 - 1.0;
    vec3 T = tangentX;
    vec3 N = normal;
    vec3 B = normalize(cross(N, T));
    T = normalize(cross(B, N));
    mat3 TBN = mat3(T, B, N);
    float normalStrength = 0.5;
    normalFromMap = mix(vec3(0, 0, 1), normalFromMap, normalStrength);
    vec3 bumpedNormal = normalize(TBN * normalFromMap);

    // Trasformazione nello spazio mondo
    vec3 worldNormal = normalize(mat3(Model2World) * bumpedNormal);
    tess_normal = vec4(worldNormal, 0.0);

    //posizione in clip space
    gl_Position = World2Camera * Model2World * p;
    vec4 worldPos = Model2World * p;
    position = worldPos.xyz;
}
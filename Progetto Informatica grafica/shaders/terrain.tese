// tessellation evaluation shader
#version 410 core

layout (quads, fractional_odd_spacing, ccw) in;

uniform sampler2D heightMap;  // the texture corresponding to our height map
uniform mat4 Model2World;
uniform mat4 World2Camera;
uniform sampler2D colorTextureNormal;

// received from Tessellation Control Shader - all texture coordinates for the patch vertices
in vec2 TextureCoord[];

// send to Fragment Shader for coloring
out float Height;

//coordinate della texture da passare al fragment
out vec2 texCoord;

out vec4 tess_normal;

out vec3 position;

void main()
{
    // get patch coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // ----------------------------------------------------------------------
    // retrieve control point texture coordinates
    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];

    // bilinearly interpolate texture coordinate across patch
    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    texCoord = (t1 - t0) * v + t0;

    // lookup texel at patch coordinate for height and scale + shift as desired
    Height = texture(heightMap, texCoord).r * 64;

    // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // bilinearly interpolate position coordinate across patch
    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec3 baseNormal = normalize(cross(uVec.xyz, vVec.xyz));

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // Modifica i fattori di scala per rendere le normali più graduali
    float heightScale = 32.0; // Riduci questo valore per normali più smooth
    float texelSize = 1.0/textureSize(heightMap, 0).x;
    
    // Campiona i punti vicini con una distanza maggiore per calcoli più stabili
    float step = 2.0 * texelSize; // Aumenta la distanza di campionamento
    
    float heightL = texture(heightMap, texCoord - vec2(step, 0.0)).r * heightScale;
    float heightR = texture(heightMap, texCoord + vec2(step, 0.0)).r * heightScale;
    float heightD = texture(heightMap, texCoord - vec2(0.0, step)).r * heightScale;
    float heightU = texture(heightMap, texCoord + vec2(0.0, step)).r * heightScale;
    
    // Calcola i vettori tangenti con valori più bilanciati
    vec3 tangentX = normalize(vec3(2.0, heightR - heightL, 0.0));
    vec3 tangentZ = normalize(vec3(0.0, heightU - heightD, 2.0));
    
    vec3 normal = normalize(cross(tangentZ, tangentX));

    // Get normal from normal map
    vec3 normalFromMap = texture(colorTextureNormal, texCoord).rgb * 2.0 - 1.0;

    // Create TBN matrix using calculated tangents
    vec3 T = tangentX;
    vec3 N = normal;
    vec3 B = normalize(cross(N, T));
    T = normalize(cross(B, N));  // Re-orthogonalize T
    mat3 TBN = mat3(T, B, N);

    // Transform normal map to world space
    vec3 finalNormal = normalize(TBN * normalFromMap);
    tess_normal = vec4(finalNormal, 0.0);

    // Apply displacement along Y axis (not along normal)
    p.y += Height;

    // ----------------------------------------------------------------------
    // output patch point position in clip space
    gl_Position = World2Camera * Model2World * p;

    vec4 worldPos = Model2World * p;
    vec3 worldNormal = mat3(Model2World) * tess_normal.xyz;
    tess_normal = vec4(normalize(worldNormal), 0.0);

    position = worldPos.xyz;
}
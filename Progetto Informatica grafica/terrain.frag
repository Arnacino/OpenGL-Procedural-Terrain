#version 410 core

// Struttura dati di lavoro per contenere le informazioni sulla luce
// ambientale
struct AmbientLightStruct {
	vec3 color;
	float intensity;
};

// Struttura dati di lavoro per contenere le informazioni sulla luce
// direzionale
struct DirectionalLightStruct {
	vec3 color;
	vec3 direction;
};

// Struttura dati di lavoro per contenere le informazioni sulla luce
// diffusiva
struct DiffusiveLightStruct {
	float intensity;
};

// Struttura dati di lavoro per contenere le informazioni sulla luce
// speculare
struct SpecularLightStruct {
	float intensity;
	float shininess;
};

uniform sampler2D ColorTextSampler;

// Questa variabile di input ha lo stesso nome di quella nel Vertex Shader
// E' importante che i nomi siano uguali perchè solo in questo modo si ha
// il passaggio delle informazioni.
//in vec3 fragment_color;

// Vettori della normali ricevuti dal tessellation shader
in vec4 tess_normal;

// Coordinate spaziali dei punti ricervuti dal vertex shader
in vec3 position;

// Coordinate di texture dei punti ricervuti dal vertex shader
in vec2 texCoord;

// Informazioni di luce ambientale 
uniform AmbientLightStruct AmbientLight;

// Informazioni di luce direzionale 
uniform DirectionalLightStruct DirectionalLight;

// Informazioni di luce diffusiva 
uniform DiffusiveLightStruct DiffusiveLight;

// Informazioni di luce speculare 
uniform SpecularLightStruct SpecularLight;

// Posizione della camera in coordinate mondo
uniform vec3 CameraPosition;

out vec4 out_color;


void main()
{

	//fattore di ripetizione della texture, applicato qui almeno non intacca la heightmap
	float repeatFactor = 1.0f;
	vec2 repeatedTexCoord = texCoord * repeatFactor;

	// La funzione texture ritorna un vec4. Nel codice noi rappresentiamo
	// i colori con vec3 e dobbiamo quindi estrarre solo 3 componenti.
	vec4 material_color = texture(ColorTextSampler, repeatedTexCoord);

	vec3 I_amb =  material_color.rgb * (AmbientLight.color * AmbientLight.intensity);

	vec3 I_dif = vec3(0,0,0);

	vec3 normal = normalize(tess_normal.xyz);

	vec3 light_dir = normalize(-DirectionalLight.direction);
	float cosTheta = dot(normal, light_dir);

	if (cosTheta > 0) {
		I_dif = material_color.rgb * (DirectionalLight.color * DiffusiveLight.intensity) * cosTheta;
	}

	vec3 I_spec = vec3(0,0,0);

	vec3 view_dir    = normalize(CameraPosition - position);
	vec3 reflect_dir = normalize(reflect(-DirectionalLight.direction,normal));

	float cosAlpha = dot(view_dir,reflect_dir);
	if (cosAlpha>0) {
		I_spec = material_color.rgb * (DirectionalLight.color * SpecularLight.intensity) * pow(cosAlpha,SpecularLight.shininess);
	}

	vec3 finalColor = clamp(I_amb + I_dif + I_spec, 0.0, 1.0);
	out_color = vec4(finalColor, material_color.a);
}
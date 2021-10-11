#define PI 3.14159265359
#define RECIPROCAL_PI 0.3183098861837697

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_world_position;
varying vec3 v_position;

uniform vec4 u_color;
uniform vec3 u_ambient;
uniform float u_light_intensity;
uniform float u_alpha;
uniform vec3 u_light_color;
uniform vec3 u_light_dir;
uniform vec3 u_light_pos;
uniform vec3 u_camera_position;
uniform samplerCube u_skybox;
uniform sampler2D u_texture;
uniform float u_active; 
vec4 color; 
vec3 light; 

uniform sampler2D u_texAlbedo; 
uniform sampler2D u_texMetal; 
uniform sampler2D u_texNormal;
uniform sampler2D u_texRough; 

const float GAMMA = 2.2;
const float INV_GAMMA = 1.0 / GAMMA;

vec3 N; 
vec3 L; 
vec3 V; 
vec3 R; 
vec3 H; 




//FUNCTIONS
void computeVectors();  
void getMaterialProperties(); 
vec4 getPixelColor(); 
//Modify normal using normal map
//vec3 perturbNormal( vec3 v_normal, vec3 view, vec2 uvs, vec3 normal_pixel )
//Sample environment levels for each component
//vec3 getReflectionColor(vec3 reflect_dir, float roughness)
struct PBRMatStruct
{
	// properties
	float roughness;
	float metalness;
	//...
	// vectors
	vec3 N;
}newMaterial;
void main()
{
	vec2 uv = v_uv;
	computeVectors(); 
	getMaterialProperties(); 

	/* Diffuse*/
	float diff = max(dot(N, L), 0.0); 
	vec3 diffuse = diff * u_light_color; 
	/*Specular*/
	float spec = pow(max(dot(V,R),0.0),u_alpha); 
	vec3 specular = spec* u_light_color; 

	light =  u_ambient+ specular +diffuse;
	 	

	color =  vec4(light,1.0)*u_light_intensity*texture2D(u_texture, uv );

	//glFragColor = getPixelColor(..)
	
	gl_FragColor = color;
}
//computeVectors:  V, N, R and H

void computeVectors()
{
	/*PHONG*/
	N = normalize(v_normal); 
	L = u_light_dir;

	V = normalize(u_camera_position -v_world_position); 
	R = reflect(-L, N); 

	H = normalize(V + L);
}
//get roughness, metalness
//	apply custom factors
//	compute diffuse and specular color (f0)

void getMaterialProperties()
{

}
//get diffuse and specular terms from direct lighting
//	get diffuse and specular terms from IBL

vec4 getPixelColor()
{
	return vec4(1.0);  //color = direct + ibl
}
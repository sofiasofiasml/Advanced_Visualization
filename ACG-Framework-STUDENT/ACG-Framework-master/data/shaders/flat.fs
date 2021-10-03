varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_world_position;
varying vec3 v_position;

uniform vec4 u_color;
//uniform sampler2D u_texture;
uniform vec3 u_ambient;
uniform float u_light_intensity;
uniform vec3 u_light_color;
uniform vec3 u_light_dir;
uniform vec3 u_light_pos;
uniform vec3 u_camera_pos;
uniform samplerCube u_skybox;
vec4 color; 
vec3 light; 
void main()
{
	vec2 uv = v_uv;
	/*PHONG*/
	vec3 N = v_normal; 
	vec3 L = u_light_dir; //normalize(v_position-u_camera_pos); 
	/* Diffuse*/
	float diff = max(dot(N, L), 0.0); 
	vec3 diffuse = diff * u_light_color; 
	/*Specular*/
	vec3 V = normalize(u_camera_pos -v_world_position); 
	vec3 R = reflect(-L, N); 
	float spec = pow(max(dot(V,R),0.0),2); 
	vec3 specular = spec* u_light_color; 

	light =  u_ambient+ specular +diffuse;
	//color =  vec4(light,1.0)* texture2D(u_texture, uv ); if we want another texture not the reflect
	color =  vec4(light,1.0)* vec4(textureCube(u_skybox,R ).rgb,1.0); 
	
	gl_FragColor = color;

}

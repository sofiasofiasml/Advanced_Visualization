varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_world_position;

uniform vec4 u_color;
uniform sampler2D Texture;
uniform vec3 u_ambient;
uniform float u_light_intensity;
uniform vec3 u_light_color;
uniform vec3 u_light_dir;
uniform vec3 u_light_pos;
uniform vec3 u_camera_pos;

vec4 color; 
vec3 light; 
void main()
{
	/*PHONG*/
	vec3 N = v_normal; 
	vec3 L = u_light_dir; 
	/* Diffuse*/
	float diff = max(dot(N, L), 0.0); 
	vec3 diffuse = diff * u_light_color; 
	/*Specular*/
	vec3 V = normalize(u_camera_pos -v_world_position); 
	vec3 R = reflect(L, N); 
	float spec = pow(max(dot(V,R),0.0),128); 
	vec3 specular = spec* u_light_color; 

	light =  u_ambient+ specular +diffuse;//; 
	color = vec4(light,1.0) * texture(Texture, v_uv); 
	gl_FragColor = color;

}

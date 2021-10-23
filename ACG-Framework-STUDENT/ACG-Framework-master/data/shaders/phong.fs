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
uniform sampler2D u_texAlbedo;
uniform int u_active; 

vec3 color; 
vec3 light; 

void main()
{
	vec2 uv = v_uv;
	/*PHONG*/
	vec3 N = normalize(v_normal); 
	vec3 L = u_light_dir;
	/* Diffuse*/
	float diff = max(dot(N, L), 0.0); 
	vec3 diffuse = diff * u_light_color; 
	/*Specular*/
	vec3 V = normalize(u_camera_position -v_world_position); 
	vec3 R = reflect(-L, N); 
	float spec = pow(max(dot(V,R),0.0),u_alpha); 
	vec3 specular = spec* u_light_color; 

	
	light =  u_ambient+ specular +diffuse;
	color =  light*u_light_intensity;

	if(u_active==1.0)
		color *= texture2D(u_texAlbedo, uv ).xyz;
	
	gl_FragColor = vec4(color,1.0f);

}

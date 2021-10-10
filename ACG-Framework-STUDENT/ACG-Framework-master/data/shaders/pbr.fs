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




void main()
{
	vec2 uv = v_uv;
	
	
	
	gl_FragColor = vec4( 1.0);
}


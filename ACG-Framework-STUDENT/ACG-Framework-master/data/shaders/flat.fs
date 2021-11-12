varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_world_position;
varying vec3 v_position;

uniform vec4 u_color;
uniform vec3 u_ambient;
uniform float u_light_intensity;
uniform vec3 u_light_color;
uniform vec3 u_light_dir;
uniform vec3 u_light_pos;
uniform vec3 u_camera_position;
uniform sampler2D u_texture;
vec4 color; 
vec3 light; 

void main()
{
	vec2 uv = v_uv;
	color =  texture2D(u_texture, uv );
	gl_FragColor = color;
}

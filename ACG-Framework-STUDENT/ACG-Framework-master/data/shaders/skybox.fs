varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_world_position;
varying vec3 v_position;

uniform vec4 u_color;
uniform samplerCube u_texture;
uniform vec3 u_ambient;
uniform float u_light_intensity;
uniform vec3 u_light_color;
uniform vec3 u_light_dir;
uniform vec3 u_light_pos;
uniform vec3 u_camera_position;

vec4 color; 
vec3 light; 

vec3 toneMap(vec3 color)
{
    return color / (color + vec3(1.0));
}

void main()
{
	vec2 uv = v_uv;
	vec3 V = normalize(v_world_position-u_camera_position ); 
	color =  textureCube(u_texture,V);
    color.xyz = toneMap(color.xyz);
	gl_FragColor = color;

}

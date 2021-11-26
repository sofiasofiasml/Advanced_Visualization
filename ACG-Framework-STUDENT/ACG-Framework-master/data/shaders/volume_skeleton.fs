#define MAX_ITERATIONS 10000000

uniform sampler3D u_texture;
uniform sampler2D u_noise;
uniform mat4 u_model;
uniform vec4 u_color;
uniform vec4 u_clipping;
uniform vec4 u_color_factor;
varying vec3 v_position;
uniform vec3 u_light_color;
uniform vec3 u_camera_position;
uniform vec3 u_light_dir; 
uniform int u_is_iso;
uniform int u_brightness;
uniform int u_is_jittering;
uniform int u_is_tf;
uniform float u_threshold; 
uniform float u_rayStep;
uniform float u_alpha;
uniform float h; 
uniform float u_density1;
uniform float u_density2;
uniform float u_density3;
uniform float u_light_intensity;
uniform int u_show_normals;

vec4 colorFinal;
vec3 rayDir;
vec3 samplePos;
vec3 coord; 
vec3 step_vector;
vec2 noise_coord;
float d; 
float noise;
vec3 normal; 
vec3 L; 
vec4 sampleColor;
float NdotL;

vec3 gradient(vec3 coord){

	float x = texture3D(u_texture, coord + vec3(h, 0.0, 0.0)).x - texture3D(u_texture, coord - vec3(h, 0.0, 0.0)).x;
	float y = texture3D(u_texture, coord + vec3(0.0, h, 0.0)).x - texture3D(u_texture, coord - vec3(0.0, h, 0.0)).x;	
	float z = texture3D(u_texture, coord + vec3(0.0, 0.0, h)).x - texture3D(u_texture, coord - vec3(0.0, 0.0, h)).x;
	vec3 gradient = -vec3(x,y,z)/(2*h);
	return gradient;
}
float isInside(){
	float result =  u_clipping.x * samplePos.x + u_clipping.y * samplePos.y + u_clipping.z * samplePos.y + u_clipping.a ;
	return result;
}
void main(){
	// 1. Ray setup
	colorFinal = vec4(0);

	//Variables at local coord
	mat3 model_local = inverse(mat3(u_model));  
	vec3 camera_pos_local = u_camera_position * model_local;  

	rayDir = normalize(v_position-camera_pos_local);
	step_vector = u_rayStep * rayDir; //pos que cambiaremos
	samplePos = v_position; //entry point

	//jittering
	noise_coord = gl_FragCoord.xy/128; //128 texture width
	noise = texture2D(u_noise, noise_coord).x;

	if (u_is_jittering == 1)
		samplePos += noise * step_vector;
	
	L =  normalize(u_light_dir); 

	// Ray loop
	for(int i=0; i<MAX_ITERATIONS; i++)
	{
		// 2. Volume sampling
		coord = (samplePos+1)/2; // Ahora va de 0 a 1, text coord
		d = texture3D(u_texture,coord).x;
		
		// 3. Classification
		if(u_is_tf == 1){
			if (d<u_density1) 
				sampleColor=vec4(0,1,0,d*u_alpha);
			else if (d<u_density2) 
				sampleColor=vec4(1,1,0,d*u_alpha);
			else if (d<u_density3) 
				sampleColor=vec4(0,0,1,d);
			else 
				sampleColor = vec4(1,1,1,d);
		}
		else {
			sampleColor = vec4(d,d,d,d);
		}
		sampleColor.rgb *= sampleColor.a;
		if(u_is_iso != 1)
			colorFinal +=  u_rayStep * (1.0 - colorFinal.a) * sampleColor;
		// 4. Composition
		if(isInside()<0){
			if(u_is_iso == 1)
			{
				if(d > u_threshold){
					normal = normalize(gradient(coord));
					NdotL = dot(normal,L)*u_light_intensity; 
					sampleColor.rgb += NdotL *u_light_color;
					colorFinal = sampleColor;
					if(u_show_normals == 1)
						colorFinal = vec4(normal,1.0);
					colorFinal.a = 1;
				}
			}
		}
	
		// 5. Next sample
		samplePos += step_vector;

		// 6. Early termination 
		if (colorFinal.a == 1 || abs(samplePos.x)>1 || abs(samplePos.y)>1 || abs(samplePos.z)>1)
			break;
		
	}
	//7. Final color 
	colorFinal*= u_color_factor *u_brightness;
	gl_FragColor = vec4(colorFinal.rgb,1.0);  
}
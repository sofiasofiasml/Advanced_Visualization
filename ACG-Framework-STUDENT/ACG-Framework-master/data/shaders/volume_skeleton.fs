#define MAX_ITERATIONS 500

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_position;

uniform vec4 u_color;
uniform vec4 u_color_factor;
uniform vec3 u_camera_position;
uniform sampler3D u_texture;
uniform float u_rayStep;
uniform int u_brightness;
uniform int u_is_jittering;
uniform int u_is_tf;
uniform sampler2D u_noise;
uniform int u_is_clip;
uniform vec4 u_clipping;
uniform int u_is_iso;
uniform float u_alpha;

vec4 colorFinal;
vec3 rayDir;
vec3 samplePos;
vec3 coord; 
vec3 step_vector;
vec2 noise_coord;
float d; 
float noise;

float isInside(){
	float result =  u_clipping.x * samplePos.x + u_clipping.y * samplePos.y + u_clipping.z * samplePos.y + u_clipping.a ;
	return result;
}
void main(){
	// 1. Ray setup
	colorFinal = vec4(0);
	rayDir = normalize(v_position-u_camera_position);
	step_vector = u_rayStep * rayDir; //pos que cambiaremos
	samplePos = v_position; //entry point
	
	//jittering
	noise_coord = gl_FragCoord.xy/128; //128 texture width
	noise = texture2D(u_noise, noise_coord).x;

	if (u_is_jittering == 1)
		samplePos += noise * rayDir;

	// Ray loop
	for(int i=0; i<MAX_ITERATIONS; i++)
	{
		// 2. Volume sampling
		coord = (samplePos+1)/2; // Ahora va de 0 a 1
		d = texture3D(u_texture,coord).x;

		// 3. Classification
		vec4 sampleColor ;
		if(u_is_tf == 1){
			sampleColor = vec4(d,d,1-d,d*d);
			sampleColor.rgb *= sampleColor.a* u_alpha;
		}
			
		else {
			sampleColor = vec4(d,d,d,d);
			sampleColor.rgb *= sampleColor.a;
		}
			
		// 4. Composition
		
		if(u_is_clip == 1)
		{
			if(isInside()<0 || isInside()==0) // si el punto se encuentra dentro del plano lo pintamos, si no se encuentra no lo pintamos
				colorFinal += u_rayStep * (1.0 - colorFinal.a) * sampleColor;
		}	
		else
			colorFinal += u_rayStep * (1.0 - colorFinal.a) * sampleColor;
		// 5. Next sample
		samplePos += step_vector;

		// 6. Early termination 
		//next sample its outside the volume
		if (colorFinal.a == 1 || abs(samplePos.x)>1 || abs(samplePos.y)>1 || abs(samplePos.z)>1)
			break;
		
		
		
	}
	//7. Final color 

	//if(colorFinal.x < 0.01) //podemos eliminar el "fondo" asi
	//	discard;
	colorFinal*= u_color_factor * u_brightness;
	gl_FragColor = vec4(colorFinal.rgb,1.0);  
}
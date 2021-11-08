#define MAX_ITERATIONS 500

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_position;

uniform vec4 u_color;
uniform vec3 u_camera_position;
uniform sampler3D u_texture;
uniform float u_rayStep;

vec3 colorFinal;
vec3 rayDir;
vec3 samplePos;
float rayStep;

void main(){
	// 1. Ray setup
	//...
	colorFinal = vec3(0);
	rayDir = normalize(v_position-u_camera_position);
	rayStep = u_rayStep;
	samplePos = v_position;

	// Ray loop
	for(int=0; i<MAX_ITERATIONS; i++){
		// 2. Volume sampling
		//...

		// 3. Classification
		//...

		// 4. Composition
		//...

		// 5. Next sample
		//...

		// 6. Early termination
		//...
	}

	//7. Final color
	//...
	
	gl_FragColor = vec4(colorFinal,1);
}
#define MAX_ITERATIONS 500

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_position;

uniform vec4 u_color;
uniform vec3 u_camera_position;
uniform sampler3D u_texture;
uniform float u_rayStep;

vec4 colorFinal;
vec3 rayDir;
vec3 samplePos;
float rayStep;

void main(){
	// 1. Ray setup
	//...
	colorFinal = vec4(0);
	rayDir = normalize(v_position-u_camera_position);
	rayStep = u_rayStep;
	samplePos = v_position;
	// Ray loop
	for(int i=0; i<MAX_ITERATIONS; i++){
		// 2. Volume sampling
		float d = texture3D(u_texture,samplePos).x;

		// 3. Classification
		vec4 sampleColor = vec4(d,d,d,d);

		// 4. Composition
		sampleColor.rgb *= sampleColor.a;
		colorFinal += rayStep * (1.0 - colorFinal.a) * sampleColor;

		// 5. Next sample
		samplePos += rayStep+i;

		// 6. Early termination
		

	}

	//7. Final color
	gl_FragColor = vec4(colorFinal.rgb,1);
}
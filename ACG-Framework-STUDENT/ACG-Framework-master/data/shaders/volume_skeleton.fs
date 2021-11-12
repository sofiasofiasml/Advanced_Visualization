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
	colorFinal = vec4(0);
	rayDir = normalize(v_position-u_camera_position);
	rayStep = u_rayStep;	//pos que cambiaremos
	samplePos = v_position; //entry point
	// Ray loop
	for(int i=0; i<MAX_ITERATIONS; i++)
	{
		// 2. Volume sampling
		vec3 coord = vec3(samplePos.x*0.5+0.5,samplePos.y*0.5+0.5,samplePos.z*0.5+0.5); // Ahora va de 0 a 1
		float d = texture3D(u_texture,coord).x;

		// 3. Classification
		vec4 sampleColor = vec4(d);

		// 4. Composition
		sampleColor.rgb *= sampleColor.a;
		colorFinal += rayStep * (1.0 - colorFinal.a) * sampleColor;

		// 5. Next sample
		samplePos = samplePos + vec3(rayStep);

		// 6. Early termination 
		//next sample its outside the volume
		//if (colorFinal.a == 1)
			//break;
	}

	//7. Final color colorFinal
	gl_FragColor = vec4(1,0,0,1.0); //Añadir u_color and brightness en imgui  vec4(colorFinal,1.0);
}
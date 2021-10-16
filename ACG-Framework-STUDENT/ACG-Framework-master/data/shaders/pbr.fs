#define PI 3.14159265359
#define RECIPROCAL_PI 0.3183098861837697
uniform samplerCube u_albedo;
uniform samplerCube u_texture_prem_0;
uniform samplerCube u_texture_prem_1;
uniform samplerCube u_texture_prem_2;
uniform samplerCube u_texture_prem_3;
uniform samplerCube u_texture_prem_4;

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
uniform float u_Metal; 
uniform float u_Rough; 
uniform float u_Normal; 
//uniform vec3 u_difuse;
vec4 color; 
vec3 light; 

uniform sampler2D u_texAlbedo; 
uniform sampler2D u_texMetal; 
uniform sampler2D u_texNormal;
uniform sampler2D u_texRough; 


vec3 N; 
vec3 L; 
vec3 V; 
vec3 R; 
vec3 H; 
vec2 uv;

const float GAMMA = 2.2;
const float INV_GAMMA = 1.0 / GAMMA;

struct PBRMatStruct
{
	// properties
	float roughness;
	float metalness;

	vec3 metal;
	vec3 rough;
	vec3 N;
	vec3 F0; 
	vec3 light;
	vec3 DifussedDirect;

}newMaterial;

// degamma
vec3 gamma_to_linear(vec3 color)
{
	return pow(color, vec3(GAMMA));
}

// gamma
vec3 linear_to_gamma(vec3 color)
{
	return pow(color, vec3(INV_GAMMA));
}

vec3 getReflectionColor(vec3 r, float roughness)
{
	float lod = roughness * 5.0;

	vec4 color;

	if(lod < 1.0) color = mix( textureCube(u_albedo, r), textureCube(u_texture_prem_0, r), lod );
	else if(lod < 2.0) color = mix( textureCube(u_texture_prem_0, r), textureCube(u_texture_prem_1, r), lod - 1.0 );
	else if(lod < 3.0) color = mix( textureCube(u_texture_prem_1, r), textureCube(u_texture_prem_2, r), lod - 2.0 );
	else if(lod < 4.0) color = mix( textureCube(u_texture_prem_2, r), textureCube(u_texture_prem_3, r), lod - 3.0 );
	else if(lod < 5.0) color = mix( textureCube(u_texture_prem_3, r), textureCube(u_texture_prem_4, r), lod - 4.0 );
	else color = textureCube(u_texture_prem_4, r);

	//color.rgb = linear_to_gamma(color.rgb);

	return color.rgb;
	return vec4(1.0f);
}

//Javi Agenjo Snipet for Bump Mapping
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv){
	// get edge vectors of the pixel triangle
	vec3 dp1 = dFdx( p );
	vec3 dp2 = dFdy( p );
	vec2 duv1 = dFdx( uv );
	vec2 duv2 = dFdy( uv );

	// solve the linear system
	vec3 dp2perp = cross( dp2, N );
	vec3 dp1perp = cross( N, dp1 );
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	// construct a scale-invariant frame
	float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
	return mat3( T * invmax, B * invmax, N );
}

vec3 perturbNormal( vec3 N, vec3 V, vec2 texcoord, vec3 normal_pixel ){
	#ifdef USE_POINTS
	return N;
	#endif

	// assume N, the interpolated vertex normal and
	// V, the view vector (vertex to eye)
	//vec3 normal_pixel = texture2D(normalmap, texcoord ).xyz;
	normal_pixel = normal_pixel * 255./127. - 128./127.;
	mat3 TBN = cotangent_frame(N, V, texcoord);
	return normalize(TBN * normal_pixel);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


/* 
	Convert 0-Inf range to 0-1 range so we can
	display info on screen
*/
vec3 toneMap(vec3 color)
{
    return color / (color + vec3(1.0));
}

// Uncharted 2 tone map
// see: http://filmicworlds.com/blog/filmic-tonemapping-operators/
vec3 toneMapUncharted2Impl(vec3 color)
{
    const float A = 0.15;
    const float B = 0.50;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.02;
    const float F = 0.30;
    return ((color*(A*color+C*B)+D*E)/(color*(A*color+B)+D*F))-E/F;
}

vec3 toneMapUncharted(vec3 color)
{
    const float W = 11.2;
    color = toneMapUncharted2Impl(color * 2.0);
    vec3 whiteScale = 1.0 / toneMapUncharted2Impl(vec3(W));
    return color * whiteScale;
}

void computeVectors()
{
	newMaterial.F0 = vec3(0); 

	N = normalize(v_normal); 
	L = u_light_dir;

	V = normalize(u_camera_position - v_world_position); 
	R = reflect(-L, N); 

	H = normalize(V + L);
}

void getMaterialProperties(){
	//Direct light
	//newMaterial.DifussedDirect = u_difuse/PI;
	

	//Indirecta
	newMaterial.metalness = texture2D(u_texMetal, uv).w ; //homogeneous vertex coordinate
	newMaterial.roughness = texture2D(u_texRough, uv).w ;

	if (newMaterial.metalness != 0)
		newMaterial.F0 = vec3(0.04f);
	
	newMaterial.F0 = vec3(0.5f);
	float cosTheta = dot(v_world_position, N);
	newMaterial.rough =  FresnelSchlickRoughness(cosTheta, newMaterial.F0, newMaterial.roughness)*u_Rough;
	vec3 color = texture2D(u_texture, uv).xyz;
	vec3 diffuseColor = (1.0 - newMaterial.metalness) * color;
	vec3 Fd_d = diffuseColor * dot(N,L);
	light = Fd_d +  newMaterial.rough;
	light =  newMaterial.rough;
}
//get diffuse and specular terms from direct lighting
//get diffuse and specular terms from IBL

vec4 getPixelColor()
{
	return vec4(newMaterial.light, 1.0f);  //color = direct + ibl
}
void main()
{
	// 1. Create Material
	// ...
	newMaterial;
	uv = v_uv;
	computeVectors(); 
	vec3 normal_pixel = texture2D(u_texNormal,uv).xyz;
	vec3 normal = perturbNormal( N, V, uv, normal_pixel);
	N = normal;
	// 2. Fill Material
	// ...
	getMaterialProperties();
	
	// 3. Shade (Direct + Indirect)
	// ...

	// 4. Apply Tonemapping
	// ...

	// 5. Any extra texture to apply after tonemapping
	// ...
	
	
	//normal = normal - v_normal;
	

	// Last step: to gamma space
	// ...
	gl_FragColor =  texture2D(u_texture, uv); //* getPixelColor();
}
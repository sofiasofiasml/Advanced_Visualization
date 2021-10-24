#define PI 3.14159265359
#define RECIPROCAL_PI 0.3183098861837697

uniform samplerCube u_texture_prem;
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
uniform float u_active; 
uniform float u_Metal; 
uniform float u_Rough; 
uniform float u_Normal;
vec4 color; 
vec3 light; 

uniform sampler2D u_texAlbedo;
uniform sampler2D u_texMetal; 
uniform sampler2D u_texNormal;
uniform sampler2D u_texRough; 
uniform sampler2D u_opacity; 
uniform sampler2D u_brdf; 
uniform sampler2D u_ao; 
uniform sampler2D u_emissive; 

//imGui
uniform int u_is_normal;
uniform int u_is_opacity;
uniform int u_is_ao;
uniform int u_is_emissive;
uniform int u_is_helmet;

vec3 L; 
vec3 N; 
vec3 V; 
vec3 R; 
vec3 H; 
vec3 F;
vec3 emissive ;
vec2 uv;

float NdotV;
float NdotH;
float LdotH;
float NdotL;

const float GAMMA = 2.2;
const float INV_GAMMA = 1.0 / GAMMA;

struct PBRMatStruct
{
	// properties
	float roughness;
	float metalness;
	float cosTheta ;

	vec3 FresnelS;
	vec3 FresnelD;
	vec3 F0S; 
	vec3 F0D; 
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

	if(lod < 1.0) color = mix( textureCube(u_texture_prem, r), textureCube(u_texture_prem_0, r), lod );
	else if(lod < 2.0) color = mix( textureCube(u_texture_prem_0, r), textureCube(u_texture_prem_1, r), lod - 1.0 );
	else if(lod < 3.0) color = mix( textureCube(u_texture_prem_1, r), textureCube(u_texture_prem_2, r), lod - 2.0 );
	else if(lod < 4.0) color = mix( textureCube(u_texture_prem_2, r), textureCube(u_texture_prem_3, r), lod - 3.0 );
	else if(lod < 5.0) color = mix( textureCube(u_texture_prem_3, r), textureCube(u_texture_prem_4, r), lod - 4.0 );
	else color = textureCube(u_texture_prem_4, r);

	//color.rgb = linear_to_gamma(color.rgb);

	return color.rgb;
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
	N = normalize(v_normal); 
	L = normalize(u_light_dir);

	V = normalize(u_camera_position - v_world_position); 
	R = normalize(reflect(-V, N)); 

	H = normalize(V + L);

	NdotV = dot(N,V);
	NdotH = dot(N,H);
	LdotH = dot(L,H);
	NdotL = dot(L,N);
}
// Normal Distribution Function using GGX Distribution
float D_GGX (const in float NoH, const in float linearRoughness )
{
	float a2 = linearRoughness * linearRoughness;
	float f = (NoH * NoH) * (a2 - 1.0) + 1.0;
	return a2 / (PI * f * f);
}
// Fresnel term with colorized fresnel
vec3 F_Schlick( const in float VoH, const in vec3 f0)
{
	float f = pow(1.0 - VoH, 5.0);
	return f0 + (vec3(1.0) - f0) * f;
}
// Geometry Term: Geometry masking/shadowing due to microfacets
float GGX(float NdotV, float k){
	return NdotV / (NdotV * (1.0 - k) + k);
}
	
float G_Smith( float NdotV, float NdotL, float roughness)
{
	float k = pow(roughness + 1.0, 2.0) / 8.0;
	return GGX(NdotL, k) * GGX(NdotV, k);
}

vec3 computeSpecularDirect()
{
	float a = newMaterial.roughness * newMaterial.roughness;

	// Normal Distribution Function
	float D = D_GGX( NdotH, a );
	
	// Fresnel Function
	F = F_Schlick( LdotH, newMaterial.F0S );

	// Visibility Function (shadowing/masking)
	float G = G_Smith( NdotV, LdotH, newMaterial.roughness);
		
	// Norm factor
	vec3 spec = D * G * F;
	spec /= (4.0 * NdotL * NdotV + 1e-6);  //1e^-6 para que nunca de 0/0
	return spec;

}
vec3 computeDiffuseDirect(vec3 color)
{
	//metallic materials do not have diffuse
	vec3 diffuse =  color;
	diffuse *= (1- newMaterial.FresnelD); //Energy conservation
	return diffuse;
}

//IBL
vec3 computeSpecularIBL()
{
	vec3 specularSample = getReflectionColor(R, newMaterial.roughness);
	vec2 brdf =  texture2D(u_brdf,vec2(NdotV, newMaterial.roughness)).xy;
	vec3 specularBRDF =  newMaterial.FresnelS * brdf.x + brdf.y;
	vec3 specularIBL = specularSample* specularBRDF ;
	return specularIBL;
}
vec3 computeDiffuseIBL(vec3 color)
{
	vec3 diffuseSample = getReflectionColor(N, 1.0f);
	vec3 diffuseBRDF =  color;
	vec3 diffuseIBL = diffuseSample * diffuseBRDF;
	diffuseIBL *= (1- newMaterial.FresnelD); //Energy conservation

	return diffuseIBL;
}

void getMaterialProperties(){
	if(u_is_helmet == 0){
		newMaterial.metalness = texture2D(u_texRough, uv).z * u_Metal; //homogeneous vertex coordinate
		newMaterial.roughness = texture2D(u_texRough, uv).y * u_Rough;
	}
	else{
		newMaterial.metalness = texture2D(u_texMetal, uv).z * u_Metal; //homogeneous vertex coordinate
		newMaterial.roughness = texture2D(u_texRough, uv).y * u_Rough;
	}
	

	//we compute the reflection in base to the color and the metalness
	newMaterial.F0S  = mix(vec3(0.04),color.xyz, newMaterial.metalness);
	newMaterial.F0D  = mix(vec3(0.0),color.xyz,newMaterial.metalness);
	newMaterial.cosTheta = max(NdotV,0.0);
	newMaterial.FresnelS =  FresnelSchlickRoughness(newMaterial.cosTheta, newMaterial.F0S, newMaterial.roughness);
	newMaterial.FresnelD =  FresnelSchlickRoughness(newMaterial.cosTheta, newMaterial.F0D, newMaterial.roughness);
	
	//Direct light
	vec3 specular = computeSpecularDirect();
	vec3 diffuse  = computeDiffuseDirect(color.xyz);

	//Indirecta
	vec3 specularIBL = computeSpecularIBL();
	vec3 diffuseIBL = computeDiffuseIBL(color.xyz);
	
	//Final light
	vec3 Indirect = specularIBL + diffuseIBL;
	vec3 Direct = (specular + diffuse )* NdotL ; 

	if (u_is_ao == 1){
		float ao_factor = texture2D(u_ao,uv).r;

		//we could play with the curve to have more control
		ao_factor = pow( ao_factor, 3.0 );
		Indirect *= ao_factor;
	}

	newMaterial.light =  Indirect + Direct;
	newMaterial.light *= u_light_intensity;

}
//get diffuse and specular terms from direct lighting
//get diffuse and specular terms from IBL

vec4 getPixelColor()
{
	
	if (u_is_emissive == 1)
		newMaterial.light += emissive;
	return vec4( newMaterial.light, 1.0f);  //color = direct + ibl
}
void main()
{
	// 1. Create Material
	newMaterial;
	uv = v_uv;
	computeVectors(); 

	//normal
	vec3 normal_pixel = texture2D(u_texNormal,uv).xyz;
	vec3 normal = perturbNormal( N, V, uv, normal_pixel);
	if (u_is_normal == 1)
		N = normal;

	//degamma emissive and albedo
	emissive = texture2D(u_emissive,uv).xyz;
	color = texture2D(u_texAlbedo, uv);
	color.xyz = gamma_to_linear(color.xyz);
	emissive.xyz = gamma_to_linear(emissive.xyz);

	// 2. Fill Material
	getMaterialProperties();
	
	// 3. Shade (Direct + Indirect)
	vec4 colorfinal =  getPixelColor();

	// 4. Apply Tonemapping
	colorfinal.xyz = toneMap(colorfinal.xyz);

	// 5. Any extra texture to apply after tonemapping
	
	if (u_is_opacity == 1)
	{	
		float opacity = texture2D(u_opacity,uv).r;
		if(opacity != 1)
			colorfinal.a = opacity;
	}

	// Last step: to gamma space
	//colorfinal.xyz = linear_to_gamma(colorfinal.xyz);
	gl_FragColor = colorfinal;
	
		

}
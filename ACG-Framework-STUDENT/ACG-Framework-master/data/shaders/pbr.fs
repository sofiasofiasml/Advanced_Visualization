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


vec3 N; 
vec3 L; 
vec3 V; 
vec3 R; 
vec3 H; 
vec2 uv;
float NdotV ; 
const float GAMMA = 2.2;
const float INV_GAMMA = 1.0 / GAMMA;
vec3 F;
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
	L = normalize(u_light_dir);

	V = normalize(u_camera_position - v_world_position); 
	R = reflect(-L, N); 

	H = normalize(V + L);
	NdotV = dot(N,V);
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
	float NoH = dot(N,H);

	// Normal Distribution Function
	float D = D_GGX( NoH, a );
	float LoH = dot(L,H);
	float NoL = dot(-L,N);
	float NoV = dot(V,N);
	// Fresnel Function
	F = F_Schlick( LoH, newMaterial.F0 );

	// Visibility Function (shadowing/masking)
	float G = G_Smith( NoV, NoL, newMaterial.roughness );
		
	// Norm factor
	vec3 spec = D * G * F;
	spec /= (4.0 * NoL * NoV + 1e-6);

	return spec;

}
vec3 computeDiffuseDirect(vec3 color)
{
	//metallic materials do not have diffuse
	vec3 diffuse = (1-newMaterial.metalness)* color;
	float NoL = dot(L,N);
	diffuse = diffuse * NoL;
	return diffuse;
}

//IBL
vec3 computeSpecularIBL()
{
	vec3 specularSample = getReflectionColor(R, newMaterial.roughness);
	vec3 specularBRDF = newMaterial.rough * texture2D(u_brdf,vec2(NdotV, newMaterial.roughness)).x * texture2D(u_brdf,vec2(NdotV, newMaterial.roughness)).y ;
	vec3 specularIBL = specularSample * specularBRDF;
	return specularIBL;
}
vec3 computeDiffuseIBL(vec3 color)
{
	vec3 diffuseSample = getReflectionColor(N, newMaterial.metalness);
	vec3 diffuseBRDF =  color;
	vec3 diffuseIBL = diffuseSample * diffuseBRDF;
	return diffuseIBL;
}

void getMaterialProperties(){
	newMaterial.metalness = texture2D(u_texMetal, uv).r ; //homogeneous vertex coordinate
	newMaterial.roughness = texture2D(u_texRough, uv).w ;
	vec3 color = texture2D(u_texture, uv).xyz;

	//we compute the reflection in base to the color and the metalness
	newMaterial.F0  = mix( vec3(0.04), color.xyz, newMaterial.metalness );

	float cosTheta = max(dot(N, V),0.0);
	newMaterial.rough =  FresnelSchlickRoughness(cosTheta, newMaterial.F0, newMaterial.roughness)*u_Rough;

	//Direct light
	vec3 specular = computeSpecularDirect();
	vec3 diffuse  = computeDiffuseDirect(color);

	//Indirecta
	vec3 specularIBL = computeSpecularIBL();
	vec3 difusseIBL = computeDiffuseIBL(color);
	vec3 F_aux = FresnelSchlickRoughness(cosTheta, newMaterial.F0, newMaterial.metalness);
	difusseIBL *= (1- F_aux); //Energy conservation
	

	float ao = texture2D(u_ao,uv).r;
	vec3 emissive = texture2D(u_emissive,uv).xyz;
	//we could play with the curve to have more control

	vec3 Indirect = specularIBL + difusseIBL;
	if (u_is_ao == 1)
		Indirect = ao * Indirect;
	//Final -MURIPLICAR POR COLOR INTENSIDAD..
	newMaterial.light = specular + diffuse + Indirect;
	if (u_is_emissive == 1)
		newMaterial.light += emissive;
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
	if (u_is_normal == 1)
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
	float opacity = texture2D(u_opacity,uv).r;
	
	
	//normal = normal - v_normal;
	

	// Last step: to gamma space
	// ...
	vec4 color = texture2D(u_texture, uv);
	gl_FragColor =  color * getPixelColor();

	if (u_is_opacity == 1)
	{
		if(opacity != 1) //no acabado
			gl_FragColor.a = opacity;
	}
		

}
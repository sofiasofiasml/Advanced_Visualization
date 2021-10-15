#pragma once
#include "material.h"
#include "texture.h"

#define LEVEL 5
class yourmaterial : public StandardMaterial
{
public:
	enum shaders { TEXTURE, PHONG, REFLECTIVE, PBR};
	shaders eMaterial;

	enum tex_material {HELMET, BALL, BENCH}; 
	tex_material eTexture; 
	
	//Textures
	Texture* text_helmet = new Texture();
	Texture* text_ball = new Texture();
	Texture* text_bench = new Texture();

	//Material Texture PBR
	Texture* tex_albedo;
	Texture* tex_normal;
	Texture* tex_rough;
	Texture* tex_metal;

	/*Texture* hdre_tex;
	Texture* hdre_tex_1;
	Texture* hdre_tex_2;
	Texture* hdre_tex_3;
	Texture* hdre_tex_4;*/

	Texture* hdr_tex[LEVEL];

	HDRE* hdre;


	//Factors PBR
	float roughness_factor; 
	float metal_factor; 
	float normal_factor; 

	//Mesh
	Mesh* meshHelmet = NULL;
	Mesh* meshSphere = NULL;
	Mesh* meshBench = NULL;
	
	// Shader
	Shader* shader_flat = NULL;
	Shader* shader_phong = NULL;
	Shader* shader_reflective = NULL;
	Shader* shader_pbr = NULL;

	//Var for the imGui options
	float u_active; 
	bool active_bool; 

	yourmaterial();
	void renderInMenu();
	void loadHdr();
};
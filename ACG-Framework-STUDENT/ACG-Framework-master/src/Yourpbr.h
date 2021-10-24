#pragma once
#include "material.h"
#include "texture.h"
#include "yourmaterial.h"

class yourpbr : public StandardMaterial
{

public:

	Texture* tex_albedo[3];
	Texture* tex_normal[2];
	Texture* tex_rough[2];
	Texture* tex_metal[2];
	Texture* ao_tex[2];

	//only one texture
	Texture* brdf_tex;
	Texture* opacity_tex;
	Texture* emissive_tex;

	int is_normal;
	int is_opacity;
	int is_ao;
	int is_emissive;
	int is_displacement; 

	float roughness_factor;
	float metal_factor;
	float desplacement_factor;
	float normal_factor; 

	yourpbr();
	void reset();
	void renderInMenu();
};
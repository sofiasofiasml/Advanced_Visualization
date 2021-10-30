#pragma once
#include "material.h"
#include "texture.h"
#include "yourmaterial.h"

class yourpbr : public StandardMaterial
{

public:
	enum output { COMPLETE, ALBEDO, NORMAL, METALNESS, ROUGHNESS };
	output eOutput;

	Texture* tex_albedo[3];
	Texture* tex_normal[3];
	Texture* tex_rough[3];
	Texture* tex_metal[3];
	Texture* ao_tex[2];

	//only one texture
	Texture* brdf_tex;
	Texture* opacity_tex;
	Texture* emissive_tex;

	//ImGUI
	int is_normal;
	int is_opacity;
	int is_ao;
	int is_emissive;
	int is_ibl;
	int is_direct;
	int ao_power;

	float roughness_factor;
	float metal_factor;
	float normal_factor; 

	yourpbr();
	void reset();
	void renderInMenu();
};
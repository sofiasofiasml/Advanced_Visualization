#pragma once
#include "material.h"
#include "texture.h"
class yourmaterial : public StandardMaterial
{
public:
	enum shaders { TEXTURE, PHONG, REFLECTIVE};
	shaders eMaterial;

	enum tex_material {HELMET, BALL, BENCH}; 
	tex_material eTexture; 
	
	//Textures
	Texture* text_helmet = new Texture();
	Texture* text_ball = new Texture();
	Texture* text_bench = new Texture();

	//Mesh
	Mesh* meshHelmet = NULL;
	Mesh* meshSphere = NULL;
	Mesh* meshBench = NULL;
	
	// Shader
	Shader* shader_flat = NULL;
	Shader* shader_phong = NULL;
	Shader* shader_reflective = NULL;

	//Var for the imGui options
	float u_active; 
	bool active_bool; 

	yourmaterial();
	void renderInMenu();

};
#pragma once
#include "material.h"
#include "texture.h"

class yourmaterial : public StandardMaterial
{
public:
	enum shaders { TEXTURE, PHONG, REFLECTIVE, PBR};
	shaders eMaterial;

	enum tex_material {HELMET, LANTERN, BALL };
	tex_material eTexture; 
	
	//Textures
	Texture* text_helmet = new Texture();
	Texture* text_ball = new Texture();
	Texture* text_bench = new Texture();

	//Mesh
	Mesh* meshHelmet = NULL;
	Mesh* meshSphere = NULL;
	Mesh* meshLantern = NULL;
	
	// Shader
	Shader* shader_flat = NULL;
	Shader* shader_phong = NULL;
	Shader* shader_reflective = NULL;
	Shader* shader_pbr = NULL;

	//Var for the imGui options
	int u_active;

	yourmaterial();
	void renderInMenu();
	
};
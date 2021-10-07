#pragma once
#include "material.h"
#include "texture.h"
class yourmaterial : public StandardMaterial
{
public:
	enum shaders { TEXTURE, PHONG, REFLECTIVE, REFRACTION };
	shaders material;

	enum tex_material { STONE, GROUND }; 
	tex_material eTexture; 
	
	Texture* text_ground = new Texture();
	Texture* text_stone = new Texture();
	//Mesh
	Mesh* meshHelmet = NULL;
	Mesh* meshSphere = NULL;
	Mesh* meshBench = NULL;
	
	// Shader
	Shader* shader_flat = NULL;
	Shader* shader_phong = NULL;
	Shader* shader_reflective = NULL;
	Shader* shader_refractive = NULL;

	yourmaterial();
	void renderInMenu();

};
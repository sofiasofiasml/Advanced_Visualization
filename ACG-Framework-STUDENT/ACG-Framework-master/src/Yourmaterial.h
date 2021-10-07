#pragma once
#include "material.h"
class yourmaterial : public StandardMaterial
{
public:
	enum shaders { TEXTURE, PHONG, REFLECTIVE, REFRACTION };
	shaders material;

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
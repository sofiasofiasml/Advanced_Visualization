#pragma once
#include "material.h"
class yourmaterial : public StandardMaterial
{
public:
	enum shaders { TEXTURE, PHONG, REFLECTIVE, REFRACTION };
	shaders material;

	yourmaterial();
	void renderInMenu();

};
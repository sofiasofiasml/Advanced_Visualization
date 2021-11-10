#pragma once
#include "material.h"
#include "texture.h"
#include "volume.h"

class volumematerial : public StandardMaterial
{
public:
	Mesh* mesh;
	Volume* volumeFoot;
	Volume* volumeBonsai;
	Volume* volumeTea;

	Texture* textureFoot;
	Texture* textureBonsai;
	Texture* textureTea;

	Volume* volume;
	Shader* shader;
	float rayStep;

	volumematerial();

	void SetUniforms(Camera* camera, Matrix44 model);
	void loadVolumeImg();
	void renderInMenu();
};
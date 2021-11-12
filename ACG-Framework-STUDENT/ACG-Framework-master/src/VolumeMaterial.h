#pragma once
#include "material.h"
#include "texture.h"
#include "volume.h"

class volumematerial : public StandardMaterial
{
public:
	enum images { FOOT, TEAPOT, BONSAI};
	images eImages;


	Mesh* mesh;
	Volume* volumeFoot;
	Volume* volumeBonsai;
	Volume* volumeTea;

	Texture* textureFoot;
	Texture* textureBonsai;
	Texture* textureTea;

	Shader* shader;
	float rayStep;
	int brightness;

	volumematerial();

	void SetUniforms(Camera* camera, Matrix44 model);
	void loadVolumeImg();
	void renderInMenu();
};
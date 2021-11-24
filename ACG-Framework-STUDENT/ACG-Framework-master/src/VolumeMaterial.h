#pragma once
#include "material.h"
#include "texture.h"
#include "volume.h"

class volumematerial : public StandardMaterial
{
public:
	enum images { ABDOMEN, TEAPOT, BONSAI, ORANGE, DAISY};
	images eImages;


	Mesh* mesh;
	Volume* volumeFoot;
	Volume* volumeBonsai;
	Volume* volumeTea;
	Volume* volumeAbd;
	Volume* volumeDaisy;
	Volume* volumeOrg;

	Texture* textureFoot;
	Texture* textureBonsai;
	Texture* textureTea;
	Texture* textureAbd;
	Texture* textureDaisy;
	Texture* textureOrg;
	Texture* noise;

	Shader* shader;
	float rayStep;
	int brightness;
	//Jittering
	int is_jittering;
	//Transfer function
	int is_tf;
	float alpha;
	float density1;
	float density2;
	float density3;
	//Clipping
	int is_clipping;
	vec4 clip;
	//Isosurfaces
	int is_iso;
	float h; 
	float threshold;
	volumematerial();

	void SetUniforms(Camera* camera, Matrix44 model);
	void loadVolumeImg();
	void renderInMenu();
};
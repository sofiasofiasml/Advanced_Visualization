#pragma once

#include "framework.h"
#include "scenenode.h"
#include "texture.h"

#define LEVEL 6

class YourSkybox : public SceneNode
{
public:
	int now_sky;
	int before_sky;

	Texture* hdr_tex[LEVEL];
	Texture* tex_skybox; 
	HDRE* hdre;
	Shader* shader_skybox;

	YourSkybox(); 

	void setUniforms(Camera* camera, Matrix44 model);
	void render(Mesh* mesh, Camera* camera);
	void loadCubemap();
	void renderInMenu(); 
	void loadHdr(const char* folder);

};

#pragma once

#include "framework.h"
#include "scenenode.h"
#include "texture.h"

class YourSkybox : public SceneNode
{
public:
	/*Mesh* mesh;
	Texture* tex_skybox; */
	int now_sky;
	int before_sky;

	YourSkybox(); 
	void loadCubemap();
	void renderInMenu(); 
};

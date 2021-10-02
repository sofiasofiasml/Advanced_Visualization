#pragma once

#include "framework.h"
#include "scenenode.h"
#include "texture.h"

class YourSkybox : public SceneNode
{
public:
	Mesh* mesh;
	Texture* tex_skybox; 
	void renderSkybox();
	void loadCubemap();

	YourSkybox(); 
};

#pragma once

#include "framework.h"
#include "scenenode.h"
#include "texture.h"

class YourSkybox : public SceneNode
{
public:
	Texture* tex_skybox;
	Mesh* mesh;
	void renderSkybox();
	void loadCubemap();

	YourSkybox(); 
};

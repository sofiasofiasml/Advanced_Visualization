#pragma once

#include "framework.h"
#include "scenenode.h"
#include "texture.h"

class YourSkybox : public SceneNode
{
public:
	int now_sky;
	int before_sky;

	YourSkybox(); 
	void loadCubemap();
	void renderInMenu(); 
};

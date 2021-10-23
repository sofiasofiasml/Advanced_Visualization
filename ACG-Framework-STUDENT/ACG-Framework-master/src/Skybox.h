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
	HDRE* hdre;

	YourSkybox(); 
	void loadCubemap();
	void renderInMenu(); 
	void loadHdr(const char* folder);

};

#include "Skybox.h"



YourSkybox::YourSkybox()
{
	this->mesh->createCube();
	this->material->texture = Texture::Get("data/textures/stone.tga");
}
void YourSkybox::renderSkybox()
{
}

void YourSkybox::loadCubemap()
{
}



#include "Skybox.h"



YourSkybox::YourSkybox()
{
	StandardMaterial* mat = new StandardMaterial();
	this->mesh = new Mesh();
	this->mesh->createCube();
	this->material = mat; 
	loadCubemap();
	this->material->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/skybox.fs");
	
}
void YourSkybox::renderSkybox()
{
}

void YourSkybox::loadCubemap()
{
	this->material->texture = new Texture();
	this->material->texture->cubemapFromImages("data/environments/snow");

}



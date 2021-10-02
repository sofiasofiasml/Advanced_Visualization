#include "Skybox.h"



YourSkybox::YourSkybox()
{
	StandardMaterial* mat = new StandardMaterial();
	this->mesh = Mesh::Get("data/meshes/sphere.obj");
	this->material = mat; 
	this->material->texture = Texture::Get("data/textures/cielo.tga");
	//this->material->texture->cubemapFromImages("data/environments/city");
	this->material->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}
void YourSkybox::renderSkybox()
{
}

void YourSkybox::loadCubemap()
{
}



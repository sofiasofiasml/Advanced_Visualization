#include "Skybox.h"
#include "application.h"


YourSkybox::YourSkybox()
{
	StandardMaterial* mat = new StandardMaterial();
	this->mesh = new Mesh();
	this->mesh->createCube();
	this->material = mat; 
	this->material->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/skybox.fs");
	this->now_sky = 0;
	this->before_sky = 0;
	loadCubemap();

}
void YourSkybox::renderInMenu()
{
	ImGui::Combo("Texture", &this->now_sky, "CITY\0SNOW\0DRAGONVALE\0");
}


void YourSkybox::loadCubemap()
{
	this->material->texture = new Texture();
	
	if (this->now_sky == 0)
		this->material->texture->cubemapFromImages("data/environments/city");
	else if (this->now_sky == 1)
		this->material->texture->cubemapFromImages("data/environments/snow");
	else if (this->now_sky == 2)
		this->material->texture->cubemapFromImages("data/environments/dragonvale");
	this->before_sky = this->now_sky;

}



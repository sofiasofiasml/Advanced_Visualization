#include "Skybox.h"
#include "application.h"


YourSkybox::YourSkybox()
{
	//We create a cube mesh and associate a 3D texture with the loadCubemap
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
	//imGui
	ImGui::Combo("Texture", &this->now_sky, "CITY\0SNOW\0DRAGONVALE\0");
}


void YourSkybox::loadCubemap()
{
	//We load the 3D images and the options are for the imGui
	this->material->texture = new Texture();

	if (this->now_sky == 0)
		this->material->texture = Application::instance->material_basic->hdr_tex[4];
		//this->material->texture->cubemapFromImages("data/environments/city");
	else if (this->now_sky == 1)
		this->material->texture->cubemapFromImages("data/environments/snow");
	else if (this->now_sky == 2)
		this->material->texture->cubemapFromImages("data/environments/dragonvale");
	this->before_sky = this->now_sky;
}



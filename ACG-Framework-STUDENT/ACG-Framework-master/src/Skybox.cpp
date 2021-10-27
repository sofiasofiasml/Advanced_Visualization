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
	loadHdr("data/environments/panorama.hdre");

}
void YourSkybox::renderInMenu()
{
	yourmaterial* mat = Application::instance->material_basic;

	//imGui
	if (mat->eMaterial != mat->PBR)
		ImGui::Combo("Texture", &this->now_sky, "CITY\0SNOW\0DRAGONVALE\0");
	else
		ImGui::Combo("Texture", &this->now_sky, "BRIDGE\0PISA\0PANORAMA\0");

}


void YourSkybox::loadCubemap()
{
	//We load the 3D images and the options are for the imGui
	this->material->texture = new Texture();
	yourmaterial* mat = Application::instance->material_basic;
	yourpbr* pbr = Application::instance->material_pbr;

	if (mat->eMaterial != mat->PBR) {
		if (this->now_sky == 0)
			this->material->texture->cubemapFromImages("data/environments/city");
		else if (this->now_sky == 1)
			this->material->texture->cubemapFromImages("data/environments/snow");
		else if (this->now_sky == 2)
			this->material->texture->cubemapFromImages("data/environments/dragonvale");
	}
	else {
		if (this->now_sky == 0)
			loadHdr("data/environments/san_giuseppe_bridge.hdre");
		else if (this->now_sky == 1)
			loadHdr("data/environments/pisa.hdre");
		else if (this->now_sky == 2)
			loadHdr("data/environments/panorama.hdre");


		this->material->texture = this->hdr_tex[0];

	}
	
	this->before_sky = this->now_sky;
}

void YourSkybox::loadHdr(const char* folder)
{
	//We load the 3D images and the options are for the imGui

	this->hdre = HDRE::Get(folder);

	for (int i = 0; i < LEVEL; i++)
	{
		this->hdr_tex[i] = new Texture();
		this->hdr_tex[i]->cubemapFromHDRE(hdre, i);
	}

}


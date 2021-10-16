#include "yourmaterial.h"
#include "application.h"


yourmaterial::yourmaterial()
{
	eMaterial = TEXTURE; //we init the material at texture
	eTexture = HELMET; 
	u_active = 0; 
	active_bool = 0; 
	this->tex_albedo = new Texture();
	this->tex_normal = new Texture();
	this->tex_metal = new Texture();
	this->tex_rough = new Texture();
	this->brdf_tex = new Texture();
	this->opacity_tex = new Texture();
	this->ao_tex = new Texture();
	this->emissive_tex = new Texture();

	this->is_normal = 0;
	this->is_opacity = 0;
	this->is_ao = 0;
	this->is_emissive= 0;
	
	
	this->roughness_factor =1.0f;
	this->metal_factor = 1.05f;
	this->normal_factor= 1.0f;
	loadHdr();
}

void yourmaterial::renderInMenu()
{
	//Im Gui
	ImGui::Combo("Output", (int*)&Application::instance->material_basic->eMaterial, "TEXTURE\0PHONG\0REFLECTIVE\0PBR\0");
	ImGui::Combo("Texture", (int*)&Application::instance->material_basic->eTexture, "BALL\0HELMET\0BENCH\0");
	if (eMaterial==PHONG)
	{
		ImGui::Checkbox("Show Texture", &Application::instance->material_basic->active_bool);
		if (active_bool == 0)Application::instance->material_basic->u_active = 0.0f;
		if (active_bool == 1)Application::instance->material_basic->u_active = 1.0f;
	}
	if (eMaterial == PBR) 
	{
		ImGui::DragFloat("Metalness", &this->metal_factor, 0.01f, 0, 1.0f);
		ImGui::DragFloat("Roughness", &this->roughness_factor, 0.01f, 0, 1.0f);
		ImGui::DragFloat("Normal", &this->normal_factor, 0.01f, 0, 1.0f);
		ImGui::Checkbox("Show Normals", (bool*)&Application::instance->material_basic->is_normal);
		ImGui::Checkbox("Show Opacity", (bool*)&Application::instance->material_basic->is_opacity);
		ImGui::Checkbox("Show ambient oclussion", (bool*)&Application::instance->material_basic->is_ao);
		ImGui::Checkbox("Show emissive", (bool*)&Application::instance->material_basic->is_emissive);

	}
	
}


void yourmaterial::loadHdr()
{
	//We load the 3D images and the options are for the imGui
	
	this->hdre = HDRE::Get("data/environments/studio.hdre");

	for (int i = 0; i < LEVEL; i++)
	{
		this->hdr_tex[i]= new Texture();
		this->hdr_tex[i]->cubemapFromHDRE(hdre, LEVEL);
	}
	
}

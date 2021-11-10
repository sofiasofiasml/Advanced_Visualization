#include "yourmaterial.h"
#include "application.h"


yourmaterial::yourmaterial()
{
	eMaterial = TEXTURE; //we init the material at texture
	eTexture = HELMET; 
	u_active = 0; 
}

void yourmaterial::renderInMenu()
{
	//Im Gui
	yourmaterial* mat = Application::instance->material_basic;
	yourpbr* pbr = Application::instance->material_pbr;
	
	//Var for restart imgui skybox
	yourmaterial* eMaterial_aux = new yourmaterial();
	eMaterial_aux->eMaterial = Application::instance->material_basic->eMaterial;
	eMaterial_aux->eTexture = Application::instance->material_basic->eTexture;

	ImGui::Combo("Output", (int*)&mat->eMaterial, "TEXTURE\0PHONG\0REFLECTIVE\0PBR\0VOLUME");
	ImGui::Combo("Texture", (int*)&mat->eTexture, "HELMET\0LANTERN\0BALL\0");
	if (eMaterial==PHONG)
		ImGui::Checkbox("Show Texture",(bool*) &mat->u_active);
	
	if(eMaterial != eMaterial_aux->eMaterial)
		Application::instance->skybox->loadCubemap();

	if (eTexture != eMaterial_aux->eTexture)
		pbr->reset();
	
}


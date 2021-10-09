#include "yourmaterial.h"
#include "application.h"


yourmaterial::yourmaterial()
{
	eMaterial = TEXTURE; //we init the material at texture
	eTexture = BALL; 
	u_active = 0; 
	active_bool = 0; 
}

void yourmaterial::renderInMenu()
{
	//Im Gui
	ImGui::Combo("Output", (int*)&Application::instance->material_basic->eMaterial, "TEXTURE\0PHONG\0REFLECTIVE\0");
	ImGui::Combo("Texture", (int*)&Application::instance->material_basic->eTexture, "BALL\0HELMET\0BENCH\0");
	if (eMaterial==PHONG)
	{
		ImGui::Checkbox("Show Texture", &Application::instance->material_basic->active_bool);
		if (active_bool == 0)Application::instance->material_basic->u_active = 0.0f;
		if (active_bool == 1)Application::instance->material_basic->u_active = 1.0f;
	}
	
}

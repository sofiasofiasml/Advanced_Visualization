#include "yourmaterial.h"
#include "application.h"


yourmaterial::yourmaterial()
{
	material = TEXTURE; //we init the material at texture
	eTexture = STONE; 
	u_active = 0; 
	active_bool = 0; 
}

void yourmaterial::renderInMenu()
{
	
	ImGui::Combo("Output", (int*)&Application::instance->material_basic->material, "TEXTURE\0PHONG\0REFLECTIVE\0\REFRACTIVE\0");
	ImGui::Combo("Texture", (int*)&Application::instance->material_basic->eTexture, "STONE\0GROUND\0");
	ImGui::Checkbox("Show Texture", &Application::instance->material_basic->active_bool);
	if (active_bool == 0)Application::instance->material_basic->u_active = 0.0f;
	if (active_bool == 1)Application::instance->material_basic->u_active = 1.0f;
}

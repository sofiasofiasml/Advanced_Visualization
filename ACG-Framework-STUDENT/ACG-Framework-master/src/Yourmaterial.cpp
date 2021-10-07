#include "yourmaterial.h"
#include "application.h"


yourmaterial::yourmaterial()
{
	material = TEXTURE; //we init the material at texture
	eTexture = STONE; 
}

void yourmaterial::renderInMenu()
{

	ImGui::Combo("Output", (int*)&Application::instance->material_basic->material, "TEXTURE\0PHONG\0REFLECTIVE\0\REFRACTIVE\0");
	ImGui::Combo("Texture", (int*)&Application::instance->material_basic->eTexture, "STONE\0GROUND\0");
}

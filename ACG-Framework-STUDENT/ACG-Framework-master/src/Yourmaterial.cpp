#include "yourmaterial.h"
#include "application.h"


yourmaterial::yourmaterial()
{
	material = TEXTURE; //we init the material at texture
}

void yourmaterial::renderInMenu()
{

	ImGui::Combo("Output", (int*)&Application::instance->material_basic->material, "TEXTURE\0PHONG\0REFLECTIVE\0\REFRACTIVE\0");
}

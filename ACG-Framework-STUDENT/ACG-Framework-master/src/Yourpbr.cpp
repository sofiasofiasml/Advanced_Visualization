#include "Yourpbr.h"
#include "application.h"


yourpbr::yourpbr()
{
	this->is_normal = 0;
	this->is_opacity = 0;
	this->is_ao = 0;
	this->is_emissive = 0;
	this->is_displacement = 0;
	this->is_ibl = 0;
	this->is_direct = 1;

	this->desplacement_factor = 1.0f; 
	this->roughness_factor = 1.0f;
	this->metal_factor = 1.0f;
	this->normal_factor = 1.0f;
	
}

void yourpbr::reset()
{
	this->is_normal = 0;
	this->is_opacity = 0;
	this->is_ao = 0;
	this->is_emissive = 0;
	this->is_displacement = 0; 

	this->desplacement_factor = 0.1f;
	this->roughness_factor = 1.0f;
	this->metal_factor = 1.0f;
	this->normal_factor = 1.0f;
}

void yourpbr::renderInMenu()
{
	yourmaterial* youmat = Application::instance->material_basic;

	if (youmat->eMaterial == youmat->PBR)
	{
		ImGui::Checkbox("Direct light", (bool*)&this->is_direct);
		ImGui::Checkbox("IBL", (bool*)&this->is_ibl);
		ImGui::DragFloat("Metalness", &this->metal_factor, 0.01f, 0, 1.0f);
		ImGui::DragFloat("Desplacement", &this->desplacement_factor, 0.01f, 0, 127.0f);
		ImGui::DragFloat("Roughness", &this->roughness_factor, 0.01f, 0, 1.0f);
		ImGui::DragFloat("Normal", &this->normal_factor, 0.01f, 0, 1.0f);
		ImGui::Checkbox("Show Normals", (bool*)&this->is_normal);
		ImGui::Checkbox("Displacement Map", (bool*)&this->is_displacement);
		
		
		if (youmat->eTexture == youmat->HELMET) {
			ImGui::Checkbox("Show ambient oclussion", (bool*)&this->is_ao);
			ImGui::Checkbox("Show emissive", (bool*)&this->is_emissive);
		}
		if (youmat->eTexture == youmat->LANTERN) {
			ImGui::Checkbox("Show ambient oclussion", (bool*)&this->is_ao);
			ImGui::Checkbox("Show Opacity", (bool*)&this->is_opacity);

		}
	}
}

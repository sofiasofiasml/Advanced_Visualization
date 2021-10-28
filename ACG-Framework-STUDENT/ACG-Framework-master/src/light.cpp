#include "Light.h"

 Light::Light() 
{
	this->color =  Vector3(1, 1, 1);
	this->ambient = Vector3(1, 1, 1);
	this->direction = Vector3(1, 1, 0); 
	this->position = Vector3(0, 20, 0); 
	this->alpha = 2;
	this->intensity = 1.0f;

 }

 void Light::renderInMenu()
 {
	 //imGui
	 yourmaterial* mat = Application::instance->material_basic;

	ImGui::DragFloat("Intensity", &this->intensity, 0.01f, 0, 1.0f);
	ImGui::DragFloat3("Direction", &this->direction.x, 0.1f, -10, 10);
	ImGui::ColorEdit4("Color", &this->color.x);

	if (mat->eMaterial == mat->PHONG) {
		ImGui::DragFloat3("Position", &this->position.x, 1, -100, 100);
		ImGui::DragFloat3("Ambient", &this->ambient.x, 0.01f, 0, 1.0f);
		ImGui::DragFloat("Alpha", &this->alpha, 1.00f, 1, 36.0f);
	}
	 
 }

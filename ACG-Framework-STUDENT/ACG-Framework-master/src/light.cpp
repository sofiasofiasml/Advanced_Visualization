#include "Light.h"

 Light::Light() 
{
	this->intensity = 1.0f; 
	this->color =  Vector3(1, 1, 1); 
	this->direction = Vector3(1, 1, 0); 
	this->position = Vector3(0, 20, 0); 
}

 void Light::renderInMenu()
 {
	 ImGui::DragFloat3("Color", &color.x, 0.1f, 0, 1);
	 ImGui::DragFloat3("Direction", &direction.x, 0.1f, -1, 1);
	 ImGui::DragFloat3("position", &position.x, 1, -100, 100);
	 ImGui::DragFloat("intensity", &this->intensity, 0.01f, 0, 1.0f);

 }

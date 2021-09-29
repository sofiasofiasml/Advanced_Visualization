#include "Light.h"

 Light::Light() 
{
	this->intensity = 1.0f; 
	this->color =  Vector3(1, 1, 1); 
	this->direction = Vector3(0, 1, 0); 
	this->position = Vector3(0, 20, 0); 
}
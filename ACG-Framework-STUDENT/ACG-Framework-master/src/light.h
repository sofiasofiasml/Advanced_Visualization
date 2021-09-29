#pragma once


#ifndef LIGHT_H
#define LIGHT_H

#include "framework.h"

class Light
{
public: 
	Vector3 color;
	float intensity;
	Vector3 direction;
	Vector3 position;

	Light();

};

#endif
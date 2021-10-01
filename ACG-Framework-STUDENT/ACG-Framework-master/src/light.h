#pragma once


#ifndef LIGHT_H
#define LIGHT_H

#include "framework.h"
#include "scenenode.h"


class Light : public SceneNode
{
public: 
	Vector3 color;
	float intensity;
	Vector3 direction;
	Vector3 position;

	Light();

};

#endif
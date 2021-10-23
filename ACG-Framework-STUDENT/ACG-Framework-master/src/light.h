#pragma once


#ifndef LIGHT_H
#define LIGHT_H

#include "framework.h"
#include "scenenode.h"
#include "application.h"


class Light : public SceneNode
{
public: 
	Vector3 color;
	Vector3 direction;
	Vector3 position;
	Vector3 ambient;
	float intensity;
	float alpha;

	Light();
	void renderInMenu(); 
};

#endif

#ifndef PBR_H
#define PBR_H

#include "framework.h"
#include "shader.h"
#include "texture.h"



class PBR {
public:


	Shader* shader; 
	PBR(); 
	void renderInMenu();
}; 

#endif
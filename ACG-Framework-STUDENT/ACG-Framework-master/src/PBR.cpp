#include "PBR.h"
#include "PBR.h"

PBR::PBR()
{
	
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/pbr.fs");
}

void PBR::renderInMenu()
{

}

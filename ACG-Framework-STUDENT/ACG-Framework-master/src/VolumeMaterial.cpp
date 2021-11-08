#include "VolumeMaterial.h"

volumematerial::volumematerial()
{
	this->shader  = Shader::Get("data/shaders/basic.vs", "data/shaders/volume_skeleton.fs");
	this->mesh->createCube();
	this->texture = new Texture();
	this->volumeFoot = new Volume();
	this->volumeBonsai = new Volume();
	this->volumeTea = new Volume();
	this->rayStep = 0.1;
	this->volume = this->volumeFoot;
}

void volumematerial::render(Camera* camera, Matrix44 model)
{
	if (this->mesh && this->shader)
	{
		//enable shader
		this->shader->enable();

		//upload uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		this->shader->disable();
	}
}

void volumematerial::SetUniforms(Camera* camera, Matrix44 model)
{
	this->shader->setUniform("u_camera_position", camera->eye);
	this->shader->setUniform("u_model", model);
	this->shader->setUniform("u_rayStep", this->rayStep);

	if ( this->volume)
		this->shader->setUniform("u_texture", this->texture, 0);
	//falta añadir texturas

}

void volumematerial::loadVolumeImg()
{
	this->volumeFoot->loadPNG("data/volumes/foot_16_16.tga");
	this->volumeBonsai->loadPNG("data/volumes/bonsai_16_16.tga");
	this->volumeTea->loadPNG("data/volumes/teapot_16_16.tga");
	this->texture->create3DFromVolume(this->volume);
}

void volumematerial::renderInMenu()
{

	
}

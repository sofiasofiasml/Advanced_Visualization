#include "VolumeMaterial.h"

volumematerial::volumematerial()
{
	this->shader  = Shader::Get("data/shaders/basic.vs", "data/shaders/volume_skeleton.fs");
	this->mesh = new Mesh();
	this->mesh->createCube();
	this->textureFoot = new Texture();
	this->textureBonsai = new Texture();
	this->textureTea = new Texture();
	this->volumeFoot = new Volume();
	this->volumeBonsai = new Volume();
	this->volumeTea = new Volume();
	this->rayStep = 0.1;
	this->volume = this->volumeFoot;
	this->loadVolumeImg();
}


void volumematerial::SetUniforms(Camera* camera, Matrix44 model)
{
	this->shader->setUniform("u_camera_position", camera->eye);
	this->shader->setUniform("u_model", model);
	this->shader->setUniform("u_rayStep", this->rayStep);

	if ( this->textureFoot)
		this->shader->setUniform("u_texture", this->textureFoot, 0);

}

void volumematerial::render(Camera* camera, Matrix44 model, Mesh* mesh)
{
	if (mesh && this->shader)
	{
		//enable shader
		this->shader->enable();

		//upload uniforms
		this->SetUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		this->shader->disable();
	}

}

void volumematerial::loadVolumeImg()
{
	this->volumeFoot->loadPNG("data/volumes/foot_16_16.tga");
	this->volumeBonsai->loadPNG("data/volumes/bonsai_16_16.tga");
	this->volumeTea->loadPNG("data/volumes/teapot_16_16.tga");

	this->textureFoot->create3DFromVolume(this->volumeFoot);
	this->textureBonsai->create3DFromVolume(this->volumeBonsai);
	this->textureTea->create3DFromVolume(this->volumeTea);
}

void volumematerial::renderInMenu()
{

	
}

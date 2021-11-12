#include "VolumeMaterial.h"

volumematerial::volumematerial()
{
	this->shader  = Shader::Get("data/shaders/basic.vs", "data/shaders/volume_skeleton.fs");
	this->mesh = new Mesh();
	this->mesh->createCube();
	this->textureFoot = new Texture();
	this->textureBonsai = new Texture();
	this->textureTea = new Texture();
	this->texture = new Texture();
	this->volumeFoot = new Volume();
	this->volumeBonsai = new Volume();
	this->volumeTea = new Volume();
	this->rayStep = 0.1;
	this->brightness = 1;
	this->loadVolumeImg();
	this->eImages = this->FOOT;
	this->texture = this->textureFoot;
}


void volumematerial::SetUniforms(Camera* camera, Matrix44 model)
{
	this->shader->setUniform("u_camera_position", camera->eye);
	this->shader->setUniform("u_model", model);
	this->shader->setUniform("u_rayStep", this->rayStep);

	if ( this->texture)
		this->shader->setUniform("u_texture", this->texture, 0);

}


void volumematerial::loadVolumeImg()
{
	this->volumeFoot->loadPNG("data/volumes/foot_16_16.png");
	this->volumeBonsai->loadPNG("data/volumes/bonsai_16_16.png");
	this->volumeTea->loadPNG("data/volumes/teapot_16_16.png");

	this->textureFoot->create3DFromVolume(this->volumeFoot);
	this->textureBonsai->create3DFromVolume(this->volumeBonsai);
	this->textureTea->create3DFromVolume(this->volumeTea);
}

void volumematerial::renderInMenu()
{
	ImGui::DragInt("brightness", &this->brightness, 1, 0, 100);
	ImGui::DragFloat("step vector", &this->rayStep, 0.01, 0, 1);
	ImGui::Combo("Output", (int*)&this->eImages, "FOOT\0TEAPOT\0BONSAI\0");

	if (this->eImages == 0)
		this->texture = this->textureFoot;
	else if (this->eImages == 1)
		this->texture = this->textureTea;
	else
		this->texture = this->textureBonsai;

}

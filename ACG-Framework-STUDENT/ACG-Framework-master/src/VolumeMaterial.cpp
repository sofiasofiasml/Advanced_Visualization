#include "VolumeMaterial.h"

volumematerial::volumematerial()
{
	this->shader  = Shader::Get("data/shaders/basic.vs", "data/shaders/volume_skeleton.fs");
	this->mesh = new Mesh();
	this->mesh->createCube();
	this->textureFoot = new Texture();
	this->textureBonsai = new Texture();
	this->textureTea = new Texture();
	this->textureAbd = new Texture();
	this->textureDaisy = new Texture();
	this->textureOrg = new Texture();
	this->noise = new Texture;
	this->noise = Texture::Get("data/blueNoise.png");
	this->texture = new Texture();
	this->is_tf = 0;
	this->alpha = 1;
	this->is_iso = 0;
	this->h = 0.03; 
	this->density1 = 0.25;
	this->density2 = 0.5;
	this->density3 = 1;
	this->threshold = 0.01;
	this->is_clipping = 0;
	this->clip = vec4(0,0,0,0);
	this->is_jittering = 0;
	this->volumeFoot = new Volume();
	this->volumeBonsai = new Volume();
	this->volumeTea = new Volume();
	this->volumeAbd = new Volume();
	this->volumeDaisy = new Volume();
	this->volumeOrg = new Volume();
	this->rayStep = 0.02;
	this->brightness = 2;
	this->loadVolumeImg();
	this->eImages = this->ABDOMEN;
	this->texture = this->textureAbd;
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
	this->volumeAbd->loadPVM("data/volumes/CT-Abdomen.pvm");
	this->volumeOrg->loadPVM("data/volumes/Orange.pvm");
	this->volumeDaisy->loadPVM("data/volumes/Daisy.pvm");

	this->textureFoot->create3DFromVolume(this->volumeFoot, GL_CLAMP_TO_EDGE);
	this->textureBonsai->create3DFromVolume(this->volumeBonsai, GL_CLAMP_TO_EDGE);
	this->textureTea->create3DFromVolume(this->volumeTea, GL_CLAMP_TO_EDGE);
	this->textureAbd->create3DFromVolume(this->volumeAbd, GL_CLAMP_TO_EDGE);
	this->textureOrg->create3DFromVolume(this->volumeOrg, GL_CLAMP_TO_EDGE);
	this->textureDaisy->create3DFromVolume(this->volumeDaisy, GL_CLAMP_TO_EDGE);
}

void volumematerial::renderInMenu()
{
	ImGui::DragInt("brightness", &this->brightness, 1, 0, 100);
	ImGui::DragFloat("step vector", &this->rayStep, 0.01, 0, 1);
	ImGui::Combo("Output", (int*)&this->eImages, "ABDOMEN\0TEAPOT\0BONSAI\0ORANGE\0DAISY\0");
	ImGui::Checkbox("Jittering", (bool*)&this->is_jittering);
	ImGui::Checkbox("Clipping", (bool*)&this->is_clipping);
	if(is_tf != 1)
		ImGui::Checkbox("Isosurfaces", (bool*)&this->is_iso);
	if(is_iso != 1)
		ImGui::Checkbox("Transfer function", (bool*)&this->is_tf);

	if (this->is_clipping == 1)
		ImGui::DragFloat4("Clipping vector", &this->clip.x, 0.001f, -1, 1);
	if(this->is_tf == 1){
		ImGui::DragFloat("Alpha", &this->alpha, 0.01, 0, 1);
		ImGui::DragFloat("Density 1", &this->density1, 0.01, -5, 1);
		ImGui::DragFloat("Density 2", &this->density2, 0.01, -5, 1);
		ImGui::DragFloat("Density 3", &this->density3, 0.01, -5, 1);
	}
		
	if (this->is_iso == 1){
		ImGui::DragFloat("H", &this->h, 0.01, 0.01, 1);
		ImGui::DragFloat("Threshold", &this->threshold, 0.01, 0.01, 1);
	}
		
	if (this->eImages == 0)
		this->texture = this->textureAbd;
	else if (this->eImages == 1)
		this->texture = this->textureTea;
	else if (this->eImages == 2)
		this->texture = this->textureBonsai;
	else if (this->eImages == 3)
		this->texture = this->textureOrg;
	else
		this->texture = this->textureDaisy;

}

#include "Skybox.h"
#include "application.h"


YourSkybox::YourSkybox()
{
	//We create a cube mesh and associate a 3D texture with the loadCubemap
	StandardMaterial* mat = new StandardMaterial();
	this->mesh = new Mesh();
	this->mesh->createCube();
	this->material = mat; 
	this->shader_skybox = Shader::Get("data/shaders/basic.vs", "data/shaders/skybox.fs");
	this->now_sky = 0;
	this->before_sky = 0;
	this->tex_skybox = new Texture();
	loadCubemap();
	loadHdr("data/environments/panorama.hdre");

}
void YourSkybox::setUniforms(Camera* camera, Matrix44 model)
{
	//Shader* shader = Application::instance->material_basic->shader;
	Light* light_dir = Application::instance->directional;
	yourmaterial* yourMat = Application::instance->material_basic;
	//yourpbr* yourPbr = Application::instance->material_pbr;

	Application* app = Application::instance;
	//upload node uniforms
	shader_skybox->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader_skybox->setUniform("u_camera_position", camera->eye);
	shader_skybox->setUniform("u_model", model);
	shader_skybox->setUniform("u_time", Application::instance->time);
	shader_skybox->setUniform("u_output", Application::instance->output);

	shader_skybox->setUniform("u_color_factor", yourMat->color);
	shader_skybox->setUniform("u_exposure", app->scene_exposure);
	shader_skybox->setUniform("u_light_intensity", light_dir->intensity);
	shader_skybox->setUniform("u_light_dir", light_dir->direction);
	//phong
	shader_skybox->setUniform("u_ambient", light_dir->ambient);
	shader_skybox->setUniform("u_light_color", light_dir->color);
	shader_skybox->setUniform("u_light_pos", light_dir->position);
	shader_skybox->setUniform("u_alpha", light_dir->alpha);
	shader_skybox->setUniform1("u_active", yourMat->u_active);
	 
	if (this->tex_skybox)
		shader_skybox->setUniform("u_texture", this->tex_skybox, 15);
}
void YourSkybox::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	//Shader* shader = Application::instance->material_basic->shader; 

	if (mesh && shader_skybox)
	{
		//enable shader
		shader_skybox->enable();

		//upload uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader_skybox->disable();
	}
}
void YourSkybox::renderInMenu()
{
	yourmaterial* mat = Application::instance->material_basic;

	//imGui
	if (mat->eMaterial != mat->PBR)
		ImGui::Combo("Texture", &this->now_sky, "CITY\0SNOW\0DRAGONVALE\0");
	else
		ImGui::Combo("Texture", &this->now_sky, "BRIDGE\0PISA\0PANORAMA\0");

}


void YourSkybox::loadCubemap()
{
	//We load the 3D images and the options are for the imGui
	this->material->texture = new Texture();
	yourmaterial* mat = Application::instance->material_basic;
	yourpbr* pbr = Application::instance->material_pbr;

	if (mat->eMaterial != mat->PBR) {
		if (this->now_sky == 0)
			this->tex_skybox->cubemapFromImages("data/environments/city");
		else if (this->now_sky == 1)
			this->tex_skybox->cubemapFromImages("data/environments/snow");
		else if (this->now_sky == 2)
			this->tex_skybox->cubemapFromImages("data/environments/dragonvale");
	}
	else {
		if (this->now_sky == 0)
			loadHdr("data/environments/san_giuseppe_bridge.hdre");
		else if (this->now_sky == 1)
			loadHdr("data/environments/pisa.hdre");
		else if (this->now_sky == 2)
			loadHdr("data/environments/panorama.hdre");


		this->tex_skybox = this->hdr_tex[0];

	}
	
	this->before_sky = this->now_sky;
}

void YourSkybox::loadHdr(const char* folder)
{
	//We load the 3D images and the options are for the imGui

	this->hdre = HDRE::Get(folder);

	for (int i = 0; i < LEVEL; i++)
	{
		this->hdr_tex[i] = new Texture();
		this->hdr_tex[i]->cubemapFromHDRE(hdre, i);
	}

}


#include "material.h"
#include "texture.h"
#include "application.h"
#include "extra/hdre.h"

StandardMaterial::StandardMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

StandardMaterial::~StandardMaterial()
{

}

void StandardMaterial::setUniforms(Camera* camera, Matrix44 model)
{
	Application* app = Application::instance; 
	Light* light_dir = Application::instance->directional; 
	yourmaterial* yourMat = Application::instance->material_basic;
	yourpbr* yourPbr = Application::instance->material_pbr;
	YourSkybox* skybox = Application::instance->skybox;

	//upload node uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Application::instance->time);
	shader->setUniform("u_output", Application::instance->output);

	shader->setUniform("u_color", color);
	shader->setUniform("u_exposure", app->scene_exposure);
	shader->setUniform("u_light_intensity", light_dir->intensity);
	shader->setUniform("u_light_dir", light_dir->direction);

	//phong
	shader->setUniform("u_ambient", light_dir->ambient);
	shader->setUniform("u_light_color", light_dir->color);
	shader->setUniform("u_light_pos", light_dir->position);
	shader->setUniform("u_alpha", light_dir->alpha); 
	shader->setUniform1("u_active", yourMat->u_active);

	//pbr
	shader->setUniform("u_Metal", yourPbr->metal_factor);
	shader->setUniform("u_Rough", yourPbr->roughness_factor);
	shader->setUniform("height_scale", yourPbr->desplacement_factor);
	shader->setUniform("u_Normal", yourPbr->normal_factor);
	shader->setUniform("u_brdf", yourPbr->brdf_tex, 11);
	shader->setUniform("u_opacity", yourPbr->opacity_tex, 12);
	shader->setUniform1("u_is_normal", yourPbr->is_normal);
	shader->setUniform1("u_is_helmet", yourMat->eTexture); 
	
	shader->setUniform1("u_is_opacity", yourPbr->is_opacity);
	shader->setUniform1("u_is_ao", yourPbr->is_ao);
	shader->setUniform1("u_is_emissive", yourPbr->is_emissive);
	shader->setUniform1("u_is_dispacement", yourPbr->is_displacement);
	shader->setUniform("u_ao", yourPbr->ao_tex[yourMat->eTexture], 13);
	shader->setUniform("u_emissive", yourPbr->emissive_tex, 14); 


	if (this->texture)
		shader->setUniform("u_texture", this->texture,0);

	if (yourPbr->tex_albedo && yourPbr->tex_metal && yourPbr->tex_normal && yourPbr->tex_rough) {
		shader->setUniform("u_texAlbedo", yourPbr->tex_albedo[yourMat->eTexture], 1);
		shader->setUniform("u_texMetal", yourPbr->tex_metal[yourMat->eTexture], 2);
		shader->setUniform("u_texNormal", yourPbr->tex_normal[yourMat->eTexture], 3);
		shader->setUniform("u_texRough", yourPbr->tex_rough[yourMat->eTexture], 4);
	}
	if (skybox->hdr_tex)
	{
		shader->setUniform("u_texture_prem", skybox->hdr_tex[0], 5);
		shader->setUniform("u_texture_prem_0", skybox->hdr_tex[1], 6);
		shader->setUniform("u_texture_prem_1", skybox->hdr_tex[2], 7);
		shader->setUniform("u_texture_prem_2", skybox->hdr_tex[3], 8);
		shader->setUniform("u_texture_prem_3", skybox->hdr_tex[4], 9);
		shader->setUniform("u_texture_prem_4", skybox->hdr_tex[5], 10);

	}

}

void StandardMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (mesh && shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
}

void StandardMaterial::renderInMenu()
{
	ImGui::ColorEdit3("Color", (float*)&color); // Edit 3 floats representing a color
}

WireframeMaterial::WireframeMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

WireframeMaterial::~WireframeMaterial()
{

}

void WireframeMaterial::render(Mesh* mesh, Matrix44 model, Camera * camera)
{
	if (shader && mesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//enable shader
		shader->enable();

		//upload material specific uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
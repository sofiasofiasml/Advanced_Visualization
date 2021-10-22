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
	yourmaterial* youMat = Application::instance->material_basic; 

	//upload node uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Application::instance->time);
	shader->setUniform("u_output", Application::instance->output);

	shader->setUniform("u_color", color);
	shader->setUniform("u_exposure", app->scene_exposure);
	shader->setUniform("u_ambient", light_dir->ambient);
	shader->setUniform("u_light_intensity", light_dir->intensity);
	shader->setUniform("u_light_color", light_dir->color);
	shader->setUniform("u_light_dir", light_dir->direction);
	shader->setUniform("u_light_pos", light_dir->position);
	shader->setUniform("u_alpha", light_dir->alpha); 
	shader->setUniform("u_active", (float)youMat->u_active);
	shader->setUniform("u_Metal", youMat->metal_factor);
	shader->setUniform("u_Rough", youMat->roughness_factor);
	shader->setUniform("u_Normal", youMat->normal_factor);
	shader->setUniform("u_brdf", youMat->brdf_tex, 11);
	shader->setUniform("u_opacity", youMat->opacity_tex, 12);
	shader->setUniform1("u_is_normal", youMat->is_normal);
	shader->setUniform1("u_is_opacity", youMat->is_opacity);
	shader->setUniform1("u_is_ao", youMat->is_ao);
	shader->setUniform1("u_is_emissive", youMat->is_emissive);
	shader->setUniform("u_ao", youMat->ao_tex, 13);
	shader->setUniform("u_emissive", youMat->emissive_tex, 14);


	if (this->texture)
		shader->setUniform("u_texture", this->texture,0);
	if (youMat->tex_albedo && youMat->tex_metal && youMat->tex_normal && youMat->tex_rough) {
		shader->setUniform("u_texAlbedo", youMat->tex_albedo, 1);
		shader->setUniform("u_texMetal", youMat->tex_metal, 2);
		shader->setUniform("u_texNormal", youMat->tex_normal, 3);
		shader->setUniform("u_texRough", youMat->tex_rough, 4);
	}
	if (youMat->hdr_tex) 
	{
		shader->setUniform("u_texture_prem", youMat->hdr_tex[0], 5);
		shader->setUniform("u_texture_prem_0", youMat->hdr_tex[1], 6);
		shader->setUniform("u_texture_prem_1", youMat->hdr_tex[2], 7);
		shader->setUniform("u_texture_prem_2", youMat->hdr_tex[3], 8);
		shader->setUniform("u_texture_prem_3", youMat->hdr_tex[4], 9);
		shader->setUniform("u_texture_prem_4", youMat->hdr_tex[5], 10);

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
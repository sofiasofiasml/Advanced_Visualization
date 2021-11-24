#include "application.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "volume.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "extra/hdre.h"
#include "extra/imgui/imgui.h"
#include "extra/imgui/imgui_impl_sdl.h"
#include "extra/imgui/imgui_impl_opengl3.h"
#include <cmath>

bool render_wireframe = false;
Camera* Application::camera = nullptr;
Application* Application::instance = NULL;

Application::Application(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;
	render_debug = true;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;
	scene_exposure = 1;
	output = 0;

	// OpenGL flags
	glEnable(GL_CULL_FACE); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	// Create camera
	camera = new Camera();
	camera->lookAt(Vector3(5.f, 5.f, 5.f), Vector3(0.f, 0.0f, 0.f), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(45.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	material_basic = new yourmaterial();
	material_volumetric = new volumematerial();
	
	material_pbr = new yourpbr();
	{
		StandardMaterial* mat = new StandardMaterial();
		SceneNode* node = new SceneNode("Visible node");
		node->material = mat;

		SceneNode* cube_node = new SceneNode("Visible node");
		cube_node->mesh = new Mesh();
		cube_node->mesh->createCube();
		cube_node->model.translate(0, 1, 0);
		cube_node->material = mat;
		//node_list.push_back(cube_node);
		//Mesh
		material_basic->meshSphere = Mesh::Get("data/meshes/sphere.obj");
		material_basic->meshHelmet = Mesh::Get("data/models/helmet/helmet.obj");
		material_basic->meshLantern = Mesh::Get("data/models/lantern/lantern.obj");
		
		//Textures
		//helmet
		material_pbr->tex_albedo[0] = Texture::Get("data/models/helmet/albedo.tga");
		material_pbr->tex_normal[0] = Texture::Get("data/models/helmet/normal.tga");
		material_pbr->tex_metal[0] = Texture::Get("data/models/helmet/metalness.tga");
		material_pbr->tex_rough[0] = Texture::Get("data/models/helmet/roughness.tga");
		material_pbr->ao_tex[0] = Texture::Get("data/models/helmet/ao.tga");
		material_pbr->emissive_tex = Texture::Get("data/models/helmet/emissive.tga");


		//lantern
		material_pbr->tex_albedo[1] = Texture::Get("data/models/lantern/albedo.tga");
		material_pbr->tex_normal[1] = Texture::Get("data/models/lantern/normal.tga");
		material_pbr->tex_metal[1] = Texture::Get("data/models/lantern/metalness.tga");
		material_pbr->tex_rough[1] = Texture::Get("data/models/lantern/roughness.tga");
		material_pbr->ao_tex[1] = Texture::Get("data/models/lantern/ao.tga");
		material_pbr->opacity_tex = Texture::Get("data/models/lantern/opacity.tga");
		material_pbr->brdf_tex = Texture::Get("data/brdfLUT.png");

		//ball
		material_pbr->tex_albedo[2] = Texture::Get("data/models/ball/albedo.tga");
		material_pbr->tex_rough[2] = Texture::Get("data/models/ball/roughness.tga");
		material_pbr->tex_metal[2] = Texture::Get("data/models/ball/metalness.tga");
		material_pbr->tex_normal[2] = Texture::Get("data/models/ball/normal.tga");


		//Shaders
		material_basic->shader_flat = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
		material_basic->shader_phong = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.fs");
		material_basic->shader_reflective = Shader::Get("data/shaders/basic.vs", "data/shaders/reflective.fs");
		material_basic->shader_pbr = Shader::Get("data/shaders/basic.vs", "data/shaders/pbr.fs");
		material_basic->shader_volume = Shader::Get("data/shaders/basic.vs", "data/shaders/volume_skeleton.fs");
		
		node->material->texture = material_pbr->tex_albedo[0]; // sample2d in shader
		node->mesh = material_basic->meshHelmet;
		node->model.translate(0, 1, 0);
		node_list.push_back(node);


	}
	
	directional = new Light();
	skybox = new YourSkybox(); 
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Application::render(void)
{
	//set the clear color (the background color)
	glClearColor(0,0,0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set the camera as default
	camera->enable();
	if(material_basic->eMaterial != material_basic->VOLUME)
		skybox->render(skybox->mesh, camera); 
		
	//set flags
	glEnable(GL_DEPTH_TEST);	
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	if (material_basic->eMaterial == material_basic->PBR) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


//if ira aqui despues de añadir skybox a 0
	for (size_t i = 0; i < node_list.size(); i++) {
		Matrix44 model;
		model.translate(0, 1, 0);

		if (material_basic->eMaterial == material_basic->TEXTURE)
			node_list[i]->material->shader = material_basic->shader_flat;
		else if (material_basic->eMaterial == material_basic->PHONG)
			node_list[i]->material->shader = material_basic->shader_phong;
		else if (material_basic->eMaterial == material_basic->REFLECTIVE)
			node_list[i]->material->shader = material_basic->shader_reflective;
		else if (material_basic->eMaterial == material_basic->PBR)
			node_list[i]->material->shader = material_basic->shader_pbr;
		else if(material_basic->eMaterial ==material_basic->VOLUME)
			node_list[i]->material->shader = material_basic->shader_volume;
		node_list[i]->render(camera);

		if(render_wireframe)
			node_list[i]->renderWireframe(camera);

		//We change the mesh if we change the imGui options
		if(material_basic->eMaterial != material_basic->VOLUME){
			if (node_list[i]->mesh_selected == 0)
				node_list[i]->mesh = material_basic->meshHelmet;
			else if (node_list[i]->mesh_selected == 1) {
				node_list[i]->mesh = material_basic->meshLantern;
				model.scale(0.02, 0.02, 0.02);
			}
			else if (node_list[i]->mesh_selected == 2)
				node_list[i]->mesh = material_basic->meshSphere;
		}
		else 
			node_list[i]->mesh = material_volumetric->mesh; 

		//We change the texture if we change the imGui options
		node_list[i]->material->texture = material_pbr->tex_albedo[material_basic->eTexture];
		node_list[i]->model = model;

	}
	
	glDisable(GL_BLEND);

}

void Application::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * 10; //the speed is defined by the seconds_elapsed so it goes constant
	float orbit_speed = seconds_elapsed * 1;
	
	//example
	float angle = seconds_elapsed * 10.f * DEG2RAD;
	/*for (int i = 0; i < root.size(); i++) {
		root[i]->model.rotate(angle, Vector3(0,1,0));
	}*/

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT && !ImGui::IsAnyWindowHovered() 
		&& !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive())) //is left button pressed?
	{
		camera->orbit(-Input::mouse_delta.x * orbit_speed, Input::mouse_delta.y * orbit_speed);
	}

	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move fast er with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_E)) camera->move(Vector3(0.0f, -1.0f, 0.0f) * (speed / 2));
	if (Input::isKeyPressed(SDL_SCANCODE_Q)) camera->move(Vector3(0.0f, 1.0f, 0.0f) * (speed / 2));
	if (Input::isKeyPressed(SDL_SCANCODE_SPACE)) camera->moveGlobal(Vector3(0.0f, -1.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_LCTRL)) camera->moveGlobal(Vector3(0.0f,  1.0f, 0.0f) * speed);

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: render_debug = !render_debug; break;
		case SDLK_F2: render_wireframe = !render_wireframe; break;
		case SDLK_F5: Shader::ReloadAll(); break; 
	}
}

void Application::onKeyUp(SDL_KeyboardEvent event)
{
}

void Application::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Application::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Application::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Application::onMouseWheel(SDL_MouseWheelEvent event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event.type)
	{
		case SDL_MOUSEWHEEL:
		{
			if (event.x > 0) io.MouseWheelH += 1;
			if (event.x < 0) io.MouseWheelH -= 1;
			if (event.y > 0) io.MouseWheel += 1;
			if (event.y < 0) io.MouseWheel -= 1;
		}
	}

	if(!ImGui::IsAnyWindowHovered() && event.y)
		camera->changeDistance(event.y * 0.5);
}

void Application::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Application::onFileChanged(const char* filename)
{
	Shader::ReloadAll();
}

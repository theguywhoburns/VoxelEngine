#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <lib/glm/glm.hpp>
#include <lib/glm/ext.hpp>
using namespace glm;

#include <graphics/shader.hpp>
#include <graphics/texture.hpp>
#include <graphics/mesh.hpp>
#include <graphics/VoxelRenderer.hpp>
#include <window/Window.hpp>
#include <window/Events.hpp>
#include <window/Camera.hpp>
#include <loaders/png.hpp>
#include <voxels/voxel.hpp>
#include <voxels/chunk.hpp>
#include <voxels/chunks.hpp>

int WIDTH = 1280;
int HEIGHT = 720;

float vertices[] = {
		// x    y
	   -0.01f,-0.01f,
	    0.01f, 0.01f,
	   -0.01f, 0.01f,
	   0.01f, -0.01f,
};

int attrs[] = {
	2, 0 //null terminator
};

int main() {
	Window::Initialize(WIDTH, HEIGHT, "Gaem");
	Events::Initialize();

	Shader* shader = load_shader("assets/shader.vect", "assets/shader.frag");
	if (shader == nullptr){
		std::cerr << "failed to load shader" << std::endl;
		Window::Terminate();
		return 1;
	}

	Shader* crosshairShader = load_shader("assets/crosshair.vect", "assets/crosshair.frag");
	if (crosshairShader == nullptr){
		std::cerr << "failed to load crosshair shader" << std::endl;
		Window::Terminate();
		return 1;
	}

	Texture* texture = load_texture("assets/block.png");
	if (texture == nullptr){
		std::cerr << "failed to load texture" << std::endl;
		delete shader;
		Window::Terminate();
		return 1;
	}

	Chunks* chunks = new Chunks(8,1,8);
	Mesh** meshes = new Mesh*[chunks->volume];
	for(size_t i = 0; i < chunks->volume; i++) {
		meshes[i] = nullptr;
	}
	VoxelRenderer renderer(1024*1024*8);
	
	glClearColor(0.6f,0.62f,0.65f,1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mesh* crosshair = new Mesh(vertices, 4, attrs);
	Camera* camera = new Camera(vec3(0,10,20), radians(90.0f));

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float camX = 0.0f;
	float camY = 0.0f;

	float speed = 5;

	while (!Window::ShouldClose()){
		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		if (Events::JPressed(GLFW_KEY_ESCAPE)){
			Window::ShouldClose(true);
		}
		if (Events::JPressed(GLFW_KEY_TAB)){
			Events::ToggleCursor();
		}

		if (Events::Pressed(GLFW_KEY_W)){
			camera->position += camera->front * delta * speed;
		}
		if (Events::Pressed(GLFW_KEY_S)){
			camera->position -= camera->front * delta * speed;
		}
		if (Events::Pressed(GLFW_KEY_D)){
			camera->position += camera->right * delta * speed;
		}
		if (Events::Pressed(GLFW_KEY_A)){
			camera->position -= camera->right * delta * speed;
		}
		if (Events::Pressed(GLFW_KEY_SPACE)) {
			camera->position.y += delta * speed;
		}
		if (Events::Pressed(GLFW_KEY_LEFT_SHIFT)) {
			camera->position.y -= delta * speed;
		}

		if (Events::_cursor_locked){
			camY += -Events::deltaY / Window::height * 2;
			camX += -Events::deltaX / Window::height * 2;

			if (camY < -radians(89.0f)){
				camY = -radians(89.0f);
			}
			if (camY > radians(89.0f)){
				camY = radians(89.0f);
			}

			camera->rotation = mat4(1.0f);
			camera->Rotate(camY, camX, 0);
		}

		{
			vec3 end;
			vec3 norm;
			vec3 iend;
			voxel* vox = chunks->RayCast(camera->position, camera->front, 10.0f, end, norm, iend);
			if(vox != nullptr) {
				if(Events::JClicked(GLFW_MOUSE_BUTTON_1)) {
					chunks->Set((int)iend.x, (int)iend.y, (int)iend.z, 0);
				}
				if(Events::JClicked(GLFW_MOUSE_BUTTON_2)) {
					chunks->Set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 2);
				}
			}
		}
	
		Chunk* closes[27];
		for (size_t i = 0; i < chunks->volume; i++){
			Chunk* chunk = chunks->chunks[i];
			if (!chunk->modified) {
				continue;
			}
			chunk->modified = false;
			if (meshes[i] != nullptr)
				delete meshes[i];

			for (int i = 0; i < 27; i++)
				closes[i] = nullptr;
			for (size_t j = 0; j < chunks->volume; j++){
				Chunk* other = chunks->chunks[j];

				int ox = other->x - chunk->x;
				int oy = other->y - chunk->y;
				int oz = other->z - chunk->z;

				if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
					continue;

				ox += 1;
				oy += 1;
				oz += 1;
				closes[(oy * 3 + oz) * 3 + ox] = other;
			}
			Mesh* mesh = renderer.Render(chunk, (const Chunk**)closes);
			meshes[i] = mesh;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw VAO
		shader->use();
		shader->UniformMatrix("projview", camera->GetProjection()*camera->GetView());
		texture->bind();
		mat4 model(1.0f);

		for(size_t i = 0; i < chunks->volume; i++) {
			Chunk* chunk = chunks->chunks[i];
			Mesh* mesh = meshes[i];
			model = mat4(1.0f);
			model = glm::translate(model, vec3(chunk->x*CHUNK_W+0.5f, chunk->y*CHUNK_H+0.5f, chunk->z*CHUNK_D+0.5f));
			shader->UniformMatrix("model", model);
			mesh->Draw(GL_TRIANGLES);
		}

		crosshairShader->use();
		crosshair->Draw(GL_LINES);
		Window::SwapBuffers();
		Events::PullEvents();
	}

	delete shader;
	delete texture;
	delete chunks;
	delete crosshair;
	delete crosshairShader;

	Window::Terminate();
	return 0;
}
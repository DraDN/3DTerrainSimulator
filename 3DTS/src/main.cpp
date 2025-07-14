#include "gal/graphics_app_sdl_opengl.hpp"
#include "terrain_generator.hpp"
#include <SDL.h>
#include <stdexcept>

glm::uvec2 size(640, 480);
gal::GraphicsApp_SDL_OpenGL app("3DTS", size);
std::unique_ptr<TerrainGenerator> terr_gen;
std::unique_ptr<gal::renderer_opengl::Shader> shader;
gal::renderer_opengl::Camera main_camera(size, glm::vec3(80.f), glm::vec3(0.f), 45.f);

void init() {
	// SDL_Log("OpenGL version: %s", glGetString(GL_VERSION));
	glm::ivec2 terr_size(100, 100);
	terr_gen = std::make_unique<TerrainGenerator>(terr_size, GL_TEXTURE0);
	terr_gen->generate();

	std::vector<gal::renderer_opengl::ShaderInfo> shader_infos = {
		{ .path = "./res/vertex_shader.vs",
		  .type = GL_VERTEX_SHADER},
		{ .path = "./res/fragment_shader.fs",
		  .type = GL_FRAGMENT_SHADER}
	};

	std::vector<gal::renderer_opengl::ShaderAttribute> attribute_infos = {
		{ .index = 0,
		  .name = "position"},
		{ .index = 1,
		  .name = "aTexCoords"}
	};

	shader = std::make_unique<gal::renderer_opengl::Shader>(shader_infos, attribute_infos);
}

void update() {
	gal::Event_SDL_OpenGL event;
	while(app.poll_event(event)) {
		switch (event.event.type) {
			case SDL_QUIT:
				app.window->running = false;
				break;
		}
	}

	main_camera.rotate_by(0.2f, glm::vec3(0.f, 1.f, 0.f));
}

void render() {
	app.renderer->clear_color = glm::vec4(0.5f, 0.f, 1.f, 1.f);
	app.renderer->clear();
	if (!terr_gen->construct_info.constructing) {
		app.renderer->render(terr_gen->model, std::ref(*shader), std::ref(main_camera));
	}
}

void destroy() {
	return;
}

int main(int argc, char* argv[]) {
	app.set_functions(init, update, render, destroy);
	app.launch();

	return 0;
}

#include "gal/graphics_app_sdl_opengl.hpp"
#include "gal/renderer_opengl/render_target.hpp"
#include "gal/renderer_opengl/light.hpp"
#include "noiselib/log.hpp"
#include "terrain_generator.hpp"

#include <SDL.h>
#include <stdexcept>

#include "log.hpp"
#include "ui.hpp"

glm::uvec2 size(1280, 720);
gal::GraphicsApp_SDL_OpenGL app;
std::unique_ptr<TerrainGenerator> terr_gen;
std::unique_ptr<gal::renderer_opengl::Shader> shader;
gal::renderer_opengl::ControllableCamera main_camera(size, glm::vec3(80.f), glm::vec3(0.f), 30.f);
std::unique_ptr<gal::renderer_opengl::RenderTarget> opengl_output;
std::unique_ptr<gal::renderer_opengl::Light> light;

bool render_window_hovered = false;

void init() {
	opengl_output = std::make_unique<gal::renderer_opengl::RenderTarget>(GL_COLOR_ATTACHMENT0, size, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_TEXTURE1);

	glm::ivec2 terr_size(100, 100);
	terr_gen = std::make_unique<TerrainGenerator>(terr_size, GL_TEXTURE0);
	// terr_gen->noise.nodes.back().set_property("Feature Scale", 5.f);
	terr_gen->noise.scale = 15.f;
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

	ImNodes::CreateContext();

	light = std::make_unique<gal::renderer_opengl::Light>(0, glm::vec3(100.f));
	
	APP_LOG_INFO("init() window function finished succesfully");
}

void update() {
	gal::Event_SDL_OpenGL event;
	while(app.poll_event(event)) {
		switch (event.event.type) {
			case SDL_QUIT:
				app.window->running = false;
				break;
			case SDL_WINDOWEVENT:
				switch (event.event.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
						if (event.event.window.windowID == SDL_GetWindowID(app.window->handle))
							app.window->running = false;
						break;
					case SDL_WINDOWEVENT_RESIZED:
						app.resize(event.event.window.data1, event.event.window.data2);
						break;
				}
				break;
			
			case SDL_MOUSEMOTION:
				if (event.event.button.button == SDL_BUTTON_LEFT && render_window_hovered) {
					float yaw = event.event.motion.xrel;
					float pitch = -event.event.motion.yrel;
					main_camera.rotate_by(yaw, pitch);

					app.window->set_mouse_grab(true);
				} else app.window->set_mouse_grab(false);
				break;
			
			case SDL_MOUSEWHEEL:
				if (render_window_hovered) {
					main_camera.zoom(-event.event.wheel.y);
				}
				break;
		}
	}

	if (render_window_hovered) {
		const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
		glm::vec3 camera_move;

		if (keyboard_state[SDL_SCANCODE_W]) camera_move.z = 1;
		else if (keyboard_state[SDL_SCANCODE_S]) camera_move.z = -1;

		if (keyboard_state[SDL_SCANCODE_D]) camera_move.x = 1;
		else if (keyboard_state[SDL_SCANCODE_A]) camera_move.x = -1;

		if (keyboard_state[SDL_SCANCODE_R]) camera_move.y = 1;
		else if (keyboard_state[SDL_SCANCODE_F]) camera_move.y = -1;

		main_camera.move_by(camera_move);
	}
}

void ui() {
	ImGui::DockSpaceOverViewport();
	ImGui::ShowDemoWindow();
	app_ui::DrawMenuBar();
	app_ui::DrawRenderWindow();
	app_ui::DrawCameraSettingsWindow();
	app_ui::DrawMeshConfigurationWindow();
	app_ui::DrawNoiseNodeEditor();
	app_ui::DrawMaterialsWindow();
	app_ui::DrawDebugWindows();
}

void render() {
	glm::vec2 new_size = main_camera.get_size();
	if (new_size.x != opengl_output->output.size.x || new_size.y != opengl_output->output.size.y) {
		// opengl_output->output.update(new_size.x, new_size.y);
		opengl_output->resize(new_size);
	}
	app.renderer->clear(std::ref(*opengl_output));
	if (!terr_gen->construct_info.constructing.load()) {
		app.renderer->render(terr_gen->model, std::ref(*shader), std::ref(main_camera), std::ref(*opengl_output), std::ref(*light));
	} 
	gal::renderer_opengl::RenderTarget::bind_window_target(app.window->size);
}

void destroy() {
	APP_LOG_INFO("Destroying app!");
	terr_gen.reset();
	shader.reset();
	opengl_output.reset();
	light.reset();
	ImNodes::DestroyContext();
	return;
}

int main(int argc, char* argv[]) {
	init_logger();

	app.init("3DTS", size, true, SDL_WINDOW_RESIZABLE);
	app.set_functions(init, update, render, destroy, ui);
	app.launch();

	return 0;
}

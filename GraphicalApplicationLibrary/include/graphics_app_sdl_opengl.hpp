#pragma once

#include <gal.hpp>
#include <window_sdl.hpp>
#include <renderer_opengl/renderer.hpp>
#include <ui_imgui/implementation/ui_context_sdl.hpp>
#include <glm_tools.hpp>

#include <optional>
#include <memory>

struct gal::Event_SDL_OpenGL {
	SDL_Event event;
	ImGuiIO& IO;
};

class gal::GraphicsApp_SDL_OpenGL {
	public:
		// GraphicsApp_SDL_OpenGL(const char* title, glm::uvec2 size, WindowFunction init, WindowFunction update, WindowFunction render, WindowFunction destroy, UIFunction ui_render);
		// GraphicsApp_SDL_OpenGL(const char* title, glm::uvec2 size, WindowFunction init, WindowFunction update, WindowFunction render, WindowFunction destroy);
		GraphicsApp_SDL_OpenGL(const char* title, glm::uvec2 size, bool has_ui = false, uint32_t flags = 0);
		~GraphicsApp_SDL_OpenGL();

		void set_functions(WindowFunction init, WindowFunction update, WindowFunction render, WindowFunction destroy, UIFunction ui_render = nullptr);
		void poll_event(Event_SDL_OpenGL& ev);
		void launch();

		void resize(float width, float height);

		std::unique_ptr<WindowSDL> window;
		std::unique_ptr<renderer_opengl::RendererOpenGL> renderer;
		std::optional<ui_imgui::implementation::sdl::UIContext> ui;
	private:
		WindowFunction _render_intercept;
		void _init_sdl();
		void _init_glew();
		void _prepare_opengl();
		void _init_opengl();
};

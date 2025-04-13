#pragma once

#include <gal.hpp>
#include <window_sdl.hpp>
#include <renderer_opengl/renderer.hpp>
#include <ui_imgui/ui_context_sdl.hpp>
#include <glm_tools.hpp>

#include <optional>

struct gal::Event_SDL_OpenGL {
	SDL_Event event;
	ImGuiIO& IO;
};

class gal::GraphicsApp_SDL_OpenGL {
	public:
		// GraphicsApp_SDL_OpenGL(const char* title, glm::uvec2 size, WindowFunction init, WindowFunction update, WindowFunction render, WindowFunction destroy, UIFunction ui_render);
		// GraphicsApp_SDL_OpenGL(const char* title, glm::uvec2 size, WindowFunction init, WindowFunction update, WindowFunction render, WindowFunction destroy);
		GraphicsApp_SDL_OpenGL(const char* title, glm::uvec2 size);

		void poll_event(Event_SDL_OpenGL ev);
		void launch();

		WindowFunction init, update, render, destroy;
		std::optional<UIFunction> ui_render;

		WindowSDL window;
		renderer_opengl::RendererOpenGL renderer;
		std::optional<ui_imgui::implementation::sdl::UIContext> ui;
};

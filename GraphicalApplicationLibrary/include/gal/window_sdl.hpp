#pragma once

#include "gal/gal.hpp"
#include "gal/glm_tools.hpp"
#include <SDL.h>

#include <optional>
#include <functional>

typedef std::function<void()> WindowFunction;

class gal::WindowSDL {
	public:
		WindowSDL(const char* title, glm::uvec2 size, uint32_t flags = 0, WindowFunction init = nullptr, WindowFunction destroy = nullptr, WindowFunction update = nullptr, WindowFunction render = nullptr);
		~WindowSDL();

		void loop();
		void resize(float width, float height);
		void set_mouse_grab(bool grab);

		bool running;
		glm::uvec2 size;

		SDL_Window* handle;
		std::optional<SDL_GLContext> opengl_context;

		WindowFunction initialize, destroy, update, render;
	
	private:
		void _render();
};

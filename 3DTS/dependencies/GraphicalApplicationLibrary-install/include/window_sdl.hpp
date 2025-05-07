#pragma once

#include <gal.hpp>
#include <glm_tools.hpp>
#include <SDL.h>

#include <optional>
#include <functional>

// typedef void (*WindowFunction)();
typedef std::function<void()> WindowFunction;

class gal::WindowSDL {
	public:
		// WindowSDL(const char* title, glm::uvec2 size, SDL_WindowFlags flags = (SDL_WindowFlags)0);
		WindowSDL(const char* title, glm::uvec2 size, uint32_t flags = 0, WindowFunction init = nullptr, WindowFunction destroy = nullptr, WindowFunction update = nullptr, WindowFunction render = nullptr);
		// WindowSDL(const char* title, std::pair<uint16_t, uint16_t> size, WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render, SDL_WindowFlags flags = (SDL_WindowFlags)0);
		~WindowSDL();

		void loop();
		void resize(float width, float height);

		bool running, grab_mouse;
		glm::uvec2 size;

		SDL_Window* handle;
		std::optional<SDL_GLContext> opengl_context;

	// private:
		WindowFunction initialize, destroy, update, render;
	
	private:
		void _render();
};

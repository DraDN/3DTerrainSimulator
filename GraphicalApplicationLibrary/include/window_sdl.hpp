#pragma once

#include <gal.hpp>
#include <glm_tools.hpp>
#include <SDL2/SDL.h>

#include <optional>

typedef void (*WindowFunction)();

class gal::WindowSDL {
	public:
		WindowSDL(const char* title, glm::uvec2 size);
		WindowSDL(const char* title, glm::uvec2 size, WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render);
		WindowSDL(const char* title, std::pair<uint16_t, uint16_t> size, WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render);

		void loop();
		void resize(float width, float height);

		bool running, grab_mouse;
		glm::uvec2 size;

		SDL_Window* handler;
		std::optional<SDL_GLContext> opengl_context;

	// private:
		WindowFunction initialize, destroy, update, render;
};

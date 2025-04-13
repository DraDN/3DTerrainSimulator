#pragma once

#include <gal.hpp>
#include <imgui.h>
#include <SDL2/SDL.h>

typedef void (*UIFunction)();

class gal::ui_imgui::implementation::sdl::UIContext {
	public:
		UIContext(SDL_Window &window, SDL_GLContext &gl_context, UIFunction render);
		~UIContext();

		void process_events(SDL_Event &events);
		void new_frame(SDL_Window &window);
		void render();

		UIFunction render_function;
};

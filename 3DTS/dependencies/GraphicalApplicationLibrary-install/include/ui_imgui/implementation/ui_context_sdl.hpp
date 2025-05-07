#pragma once

#include <gal.hpp>
#include <imgui.h>
#include <SDL.h>

#include <functional>

typedef std::function<void()> UIFunction;

class gal::ui_imgui::implementation::sdl::UIContext {
	public:
		UIContext(SDL_Window &window, SDL_GLContext &gl_context, UIFunction render = nullptr);
		~UIContext();

		void process_events(SDL_Event &events);
		void render();

		UIFunction render_function;
	private:
		void _new_frame();

		SDL_Window* window_handler;
};

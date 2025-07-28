#include "gal/window_sdl.hpp"

gal::WindowSDL::WindowSDL(const char* title, glm::uvec2 size, uint32_t flags, WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render) :
size(size), initialize(init), destroy(destroy), update(update), render(render) {
	running = false;
	
	handle = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		size.x, size.y,
		flags);
		
		if (handle == nullptr) {
			GAL_LOG_CRITICAL("Cound't create SDL window handle!");
			exit(1);
		}
		
		GAL_LOG_INFO("Window \"{}\" created", title);
	}
	
gal::WindowSDL::~WindowSDL() {
	destroy();
	if (opengl_context.has_value())
		SDL_GL_DeleteContext(opengl_context.value());
	SDL_DestroyWindow(handle);
	GAL_LOG_INFO("SDL Window destroyed...");
}

void gal::WindowSDL::loop() {
	initialize();
	GAL_LOG_INFO("Window initialized...");

	running = true;
	while (running) {
		update();
		_render();
	}
}

void gal::WindowSDL::_render() {
	if (render == nullptr) {
		GAL_LOG_WARN("Render function not set!");
	} else {
		render();
	}

	if (opengl_context.has_value()) {
		SDL_GL_SwapWindow(handle);
	}
}

void gal::WindowSDL::resize(float width, float height) {
	size.x = width;
	size.y = height;

	SDL_SetWindowSize(handle, size.x, size.y);
}

void gal::WindowSDL::set_mouse_grab(bool grab) {
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(grab));
}

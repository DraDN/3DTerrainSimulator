#include <window_sdl.hpp>

gal::WindowSDL::WindowSDL(const char* title, glm::uvec2 size, uint32_t flags, WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render) :
	size(size), initialize(init), destroy(destroy), update(update), render(render) {
	running = false;
	grab_mouse = false;

	handle = SDL_CreateWindow(title,
							  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  size.x, size.y,
							  flags);
	
	if (handle == nullptr) {
		exit(1);
	}
}

void gal::WindowSDL::loop() {
	initialize();

	running = true;
	while (running) {
		update();
		_render();
	}
}

void gal::WindowSDL::_render() {
	render();
	if (opengl_context.has_value())
		SDL_GL_SwapWindow(handle);
}

gal::WindowSDL::~WindowSDL() {
	destroy();
	if (opengl_context.has_value())
		SDL_GL_DeleteContext(opengl_context.value());
	SDL_DestroyWindow(handle);
}

void gal::WindowSDL::resize(float width, float height) {
	size.x = width;
	size.y = height;

	SDL_SetWindowSize(handle, size.x, size.y);
}

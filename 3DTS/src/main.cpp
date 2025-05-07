#include <graphics_app_sdl_opengl.hpp>
// #include <terrain_generator.hpp>
#include <iostream>
#include <SDL.h>

glm::ivec2 size(640, 480);
gal::GraphicsApp_SDL_OpenGL app("3DTS", size);

void init() {
	std::cout << "inited!";
	SDL_Log("inited!");
	return;
}

void update() {
	SDL_Log("updated!");
	gal::Event_SDL_OpenGL event;
	// while(app.poll_event(event)) {
	while(SDL_PollEvent(&event.event)) {
		SDL_Log("got event");
		switch (event.event.type) {
			case SDL_QUIT:
				app.window->running = false;
				SDL_Log("exited!");
				break;
		}
	}
	SDL_Log("killed by update...");

	// return;
}

void render() {
	SDL_Log("render!");
	// return;
}

void destroy() {
	return;
}

int main(int argc, char* argv[]) {
	// glm::ivec2 size(640, 480);
	std::cout << "test" << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Log("test");
	app.set_functions(init, update, render, destroy);
	app.launch();

	return 0;
}

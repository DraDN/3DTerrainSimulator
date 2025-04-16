#include <graphics_app_sdl_opengl.hpp>

void gal::GraphicsApp_SDL_OpenGL::_init_sdl() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		exit(1);
	}
}

void gal::GraphicsApp_SDL_OpenGL::_init_glew() {
	glewExperimental = true;
	glewInit();
}

void gal::GraphicsApp_SDL_OpenGL::_prepare_opengl() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4.4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}

void gal::GraphicsApp_SDL_OpenGL::_init_opengl() {
	window->opengl_context.emplace(SDL_GL_CreateContext(window->handle));

	if (window->opengl_context.value() == nullptr) {
		exit(1);
	}

	SDL_GL_MakeCurrent(window->handle, window->opengl_context.value());

	_init_glew();
}

gal::GraphicsApp_SDL_OpenGL::GraphicsApp_SDL_OpenGL(const char* title, glm::uvec2 size, bool has_ui, uint32_t flags) {
	_init_sdl();
	_prepare_opengl();

	window = std::make_unique<WindowSDL>(title, size, flags | SDL_WINDOW_OPENGL);

	_init_opengl();

	renderer = std::make_unique<renderer_opengl::RendererOpenGL>(size);

	if (has_ui)
		ui.emplace(*window->handle, window->opengl_context.value());
	
	_render_intercept = [this]() {
		if (ui.has_value()) {
			ui.value().render();
		}
		window->render();
	};
}

gal::GraphicsApp_SDL_OpenGL::~GraphicsApp_SDL_OpenGL() {
	SDL_Quit();
}

void gal::GraphicsApp_SDL_OpenGL::set_functions(WindowFunction init, WindowFunction update, WindowFunction render, WindowFunction destroy, UIFunction ui_render) {
	window->initialize = init;
	window->update = update;
	window->render = render;
	window->destroy = destroy;
	if (ui.has_value() && ui_render)
		ui.value().render_function = ui_render;
}

void gal::GraphicsApp_SDL_OpenGL::poll_event(Event_SDL_OpenGL& ev) {
	SDL_PollEvent(&ev.event);
	ui.value().process_events(ev.event);
	ev.IO = ImGui::GetIO();
}

void gal::GraphicsApp_SDL_OpenGL::launch() {
	window->render = _render_intercept;
	window->loop();
}

void gal::GraphicsApp_SDL_OpenGL::resize(float width, float height) {
	window->resize(width, height);
	renderer->window_size.x = width;
	renderer->window_size.y = height;
}

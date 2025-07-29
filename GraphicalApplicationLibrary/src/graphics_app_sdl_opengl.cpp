#include "gal/graphics_app_sdl_opengl.hpp"

void gal::GraphicsApp_SDL_OpenGL::_init_sdl() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		GAL_LOG_CRITICAL("Couldn't intialize SDL!");
		exit(1);
	}

	GAL_LOG_INFO("SDL Initialized");
}

void gal::GraphicsApp_SDL_OpenGL::_init_glew() {
	glewExperimental = true;
	glewInit();

	GAL_LOG_INFO("GLEW Initialized");
}

void gal::GraphicsApp_SDL_OpenGL::_prepare_opengl() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

#ifdef GAL_LOGGING
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
}

void gal::GraphicsApp_SDL_OpenGL::_init_opengl() {
	window->opengl_context.emplace(SDL_GL_CreateContext(window->handle));

	if (window->opengl_context.value() == nullptr) {
		GAL_LOG_CRITICAL("Couldn't create OpenGL context!");
		exit(1);
	}

	SDL_GL_MakeCurrent(window->handle, window->opengl_context.value());

	_init_glew();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_MULTISAMPLE);

#ifdef GAL_LOGGING
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

	glViewport(0, 0, window->size.x, window->size.y);

	GAL_LOG_INFO("OpenGL Initialized - Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void gal::GraphicsApp_SDL_OpenGL::init(const char* title, glm::uvec2 size, bool has_ui, uint32_t flags) {
	_init_sdl();
	_prepare_opengl();

	window = std::make_unique<WindowSDL>(title, size, flags | SDL_WINDOW_OPENGL);

	_init_opengl();

	renderer = std::make_unique<renderer_opengl::RendererOpenGL>(size);

	if (has_ui)
		ui.emplace(*window->handle, window->opengl_context.value());
	
	GAL_LOG_INFO("Graphics App created");
}

gal::GraphicsApp_SDL_OpenGL::~GraphicsApp_SDL_OpenGL() {
	window.reset();
	renderer.reset();
	ui.reset();
	SDL_Quit();
	GAL_LOG_INFO("SDL quitted...");
}

void gal::GraphicsApp_SDL_OpenGL::set_functions(WindowFunction init, WindowFunction update, WindowFunction render, WindowFunction destroy, UIFunction ui_render) {
	window->initialize = init;
	window->update = update;
	window->render = render;
	window->destroy = destroy;
	if (ui.has_value() && ui_render)
		ui.value().render_function = ui_render;
	
	GAL_LOG_INFO("Window functions set...");
}

bool gal::GraphicsApp_SDL_OpenGL::poll_event(Event_SDL_OpenGL& ev) {
	bool is_event = SDL_PollEvent(&ev.event);
	if (ui.has_value() && is_event) {
		ui->process_events(ev.event);
		ev.IO = &ImGui::GetIO();
	}
	return is_event;
}

void gal::GraphicsApp_SDL_OpenGL::launch() {
	_render_intercept = window->render;
	window->render = [this]() {
		_render_intercept();
		
		if (ui.has_value()) 
			ui.value().render();
	};

	GAL_LOG_INFO("Launching application...");
	window->loop();
}

void gal::GraphicsApp_SDL_OpenGL::resize(float width, float height) {
	window->resize(width, height);
	renderer->window_size.x = width;
	renderer->window_size.y = height;

	GAL_LOG_INFO("Window resized...");
}

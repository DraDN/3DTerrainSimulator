#include <ui_imgui/implementation/ui_context_sdl.hpp>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

gal::ui_imgui::implementation::sdl::UIContext::UIContext(SDL_Window& window, SDL_GLContext& gl_context, UIFunction render) {
	window_handler = &window;
	render_function = render;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;

	const char glsl_version[] = "#version 430";
	ImGui_ImplSDL2_InitForOpenGL(window_handler, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

gal::ui_imgui::implementation::sdl::UIContext::~UIContext() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
}

void gal::ui_imgui::implementation::sdl::UIContext::process_events(SDL_Event &events) {
	ImGui_ImplSDL2_ProcessEvent(&events);
}

void gal::ui_imgui::implementation::sdl::UIContext::_new_frame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window_handler);
	ImGui::NewFrame();
}

void gal::ui_imgui::implementation::sdl::UIContext::render() {
	_new_frame();
	render();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

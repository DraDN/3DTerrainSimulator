#include "ui.hpp"
#include "gal/graphics_app_sdl_opengl.hpp"
#include "terrain_generator.hpp"
#include "log.hpp"

extern glm::uvec2 size;
extern gal::GraphicsApp_SDL_OpenGL app;
extern std::unique_ptr<TerrainGenerator> terr_gen;
extern std::unique_ptr<gal::renderer_opengl::Shader> shader;
extern gal::renderer_opengl::ControllableCamera main_camera;
extern std::unique_ptr<gal::renderer_opengl::RenderTarget> opengl_output;

ImGuiWindowFlags WindowFlags_FixedAutoSize = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
ImGuiWindowFlags WindowFlags_Overlay = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

extern bool render_window_hovered;

bool show_debug_window = false;
bool show_camera_settings_window = false;
bool show_mesh_configuration_window = false;

void app_ui::DrawRenderWindow() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (ImGui::Begin("Render Window")) {
		ImGui::BeginChild("render");
		
		render_window_hovered = ImGui::IsWindowHovered();

		ImVec2 wsize = ImGui::GetWindowSize();
		main_camera.set_size(wsize.x, wsize.y);

		ImGui::Image(reinterpret_cast<ImTextureID>(opengl_output->output.handle), wsize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::EndChild();
	}
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void app_ui::DrawMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::BeginMenu("Render")) {
				ImGui::Checkbox("Wireframe", &app.renderer->wireframe);
				ImGui::ColorEdit4("Clear color", &app.renderer->clear_color.x, ImGuiColorEditFlags_NoInputs);
				ImGui::EndMenu();
			}

			ImGui::MenuItem("Camera", NULL, &show_camera_settings_window);

			if (ImGui::BeginMenu("Debug")) {
				ImGui::MenuItem("Show debug window", NULL, &show_debug_window);
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Terrain Configuration")) {
			ImGui::MenuItem("Mesh Configuration", NULL, &show_mesh_configuration_window);
			ImGui::EndMenu();
		}

		if (terr_gen->construct_info.constructing.load() || terr_gen->construct_info.ready_to_upload.load()) {
			float progress = (float)terr_gen->construct_info.progress.load() / (float)(terr_gen->model_size.x * terr_gen->model_size.y);
			ImGui::ProgressBar(progress, ImVec2(ImGui::GetWindowWidth()/4.f, 0));
		} else {
			if (ImGui::Button("Generate Terrain")) {
				terr_gen->generate();
			}
		}

		ImGui::EndMainMenuBar();
	}
}

void app_ui::DrawCameraSettingsWindow() {
	if (!show_camera_settings_window) return;

	if (ImGui::Begin("Camera Settings", &show_camera_settings_window, WindowFlags_FixedAutoSize)) {
		glm::vec3 new_cam_pos = main_camera.get_position();
		float new_fov = main_camera.get_fov();
		if (ImGui::InputFloat3("Position", &new_cam_pos.x)) {
			main_camera.set_position(new_cam_pos);
		}
		if (ImGui::InputFloat("FOV", &new_fov)) {
			main_camera.set_fov(new_fov);
		}
		ImGui::InputFloat("Sensitivity", &main_camera.look_sensitivity, 0.001f, 0.1f);
		ImGui::InputFloat("Speed", &main_camera.move_speed, 0.001f, 0.1f);
		ImGui::End();
	}
}

void app_ui::DrawMeshConfigurationWindow() {
	if (!show_mesh_configuration_window) return;

	if (ImGui::Begin("Mesh Configuration", &show_mesh_configuration_window, WindowFlags_FixedAutoSize)) {
		glm::ivec2 input_size(terr_gen->model_size);
		if (ImGui::InputInt2("Mesh size", &input_size.x)) {
			if (input_size.x < 0) input_size.x = 0; 
			if (input_size.y < 0) input_size.y = 0; 

			terr_gen->model_size = input_size;
		}

		ImGui::InputFloat("Distance between vertecies", &terr_gen->distance_between_vertecies);
		
		std::string normal_multiplication_button_name("Normal map scale - x");
		normal_multiplication_button_name += std::to_string(terr_gen->normal_multiplication);

		if (ImGui::Button(normal_multiplication_button_name.c_str()))
			ImGui::OpenPopup("normal_mult_select_popup");
		
		if (ImGui::BeginPopup("normal_mult_select_popup")) {
			int value = -1;

			if (ImGui::Button("x1")) value = 1;
			ImGui::SameLine();
			if (ImGui::Button("x2")) value = 2;
			ImGui::SameLine();
			if (ImGui::Button("x4")) value = 4;

			if (value != -1) {
				terr_gen->normal_multiplication = value;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}

		ImGui::End();
	}
}

void app_ui::DrawDebugWindows() {
	if (!show_debug_window) return;

	if (ImGui::Begin("Debug", &show_debug_window, WindowFlags_FixedAutoSize)) {
		ImGui::Text("FPS - %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("Clear color - %f, %f, %f, %f", app.renderer->clear_color.r, app.renderer->clear_color.g, app.renderer->clear_color.b, app.renderer->clear_color.a);
		glm::vec2 size = main_camera.get_size();
		ImGui::Text("Camera size - %f, %f", size.x, size.y);
		ImGui::Text("OpenGL Output size - %d, %d", opengl_output->output.size.x, opengl_output->output.size.y);
		ImGui::Text("Is render window hovered - %d", render_window_hovered);
		ImGui::Text("Terrain is constructing - %d", terr_gen->construct_info.constructing.load());
		ImGui::Text("Terrain is ready to uploaded - %d", terr_gen->construct_info.ready_to_upload.load());
		ImGui::Text("Terrain progress - %d", terr_gen->construct_info.progress.load());
		ImGui::End();
	}
}
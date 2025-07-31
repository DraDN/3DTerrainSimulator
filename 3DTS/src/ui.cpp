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
bool show_noise_editor = false;
bool show_materials_window = false;

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
			ImGui::MenuItem("Noise Configuration", NULL, &show_noise_editor);
			ImGui::MenuItem("Material Configuration", NULL, &show_materials_window);
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

ImVec2 position_to_add_new_node;
int node_id_to_add = -1;

static void destroy_links(bool del_key_pressed) {
	std::vector<int> links_to_delete;
	int selected_link_count = ImNodes::NumSelectedLinks();

	if (selected_link_count && del_key_pressed) {
		links_to_delete.resize(selected_link_count);
		ImNodes::GetSelectedLinks(links_to_delete.data());
	}

	int destroyed_link_id;
	if (ImNodes::IsLinkDestroyed(&destroyed_link_id))
		links_to_delete.push_back(destroyed_link_id);

	for (int delete_id : links_to_delete) {
		APP_LOG_INFO("deleting link {}", delete_id);
		if (delete_id == 0) {
			terr_gen->noise.output = nullptr;
			continue;
		}

		for (auto& node : terr_gen->noise.nodes) {
			int attrib_id = node.second.get_starting_attrib_id();

			for (FastNoise::NodeData* link : node.second.get_lookups()) {
				(void)link;
				if (attrib_id == delete_id) {
					node.second.get_lookup(attrib_id) = nullptr;
					APP_LOG_INFO("deleted attrib {} from node {}", attrib_id, node.second.id);
				}
				attrib_id++;
			}
		}
	}
}

static void draw_nodes() {
	for (auto& [node_id, node] : terr_gen->noise.nodes) {
		ImNodes::BeginNode(node.id);

		auto& data = node.node_data;
		auto meta = data->metadata;

		ImNodes::BeginNodeTitleBar();
		ImGui::TextUnformatted(FastNoise::Metadata::FormatMetadataNodeName(meta).c_str());
		ImNodes::EndNodeTitleBar();
		
		ImGui::PushItemWidth(90.f);
		
		int attribute_id = node.get_starting_attrib_id();
		
		std::string member_formated_name;
		for (auto& memberNode : meta->memberNodeLookups) {
			ImNodes::BeginInputAttribute(attribute_id++);
			member_formated_name = FastNoise::Metadata::FormatMetadataMemberName(memberNode);
			ImGui::TextUnformatted(member_formated_name.c_str());
			ImNodes::EndInputAttribute();
		}

		for (size_t i = 0; i < meta->memberHybrids.size(); i++) {
			ImNodes::BeginInputAttribute(attribute_id++);

			bool is_linked = data->hybrids[i].first;
			const char* float_format = "%.3f";

			if (is_linked) {
				float_format = "";
			}

			member_formated_name = FastNoise::Metadata::FormatMetadataMemberName(meta->memberHybrids[i]);

			ImGui::DragFloat(member_formated_name.c_str(), &data->hybrids[i].second, meta->memberHybrids[i].valueUiDragSpeed, 0, 0, float_format);

			ImNodes::EndInputAttribute();
		}

		size_t members_num = meta->memberVariables.size();
		for (size_t mem_id = 0; mem_id < members_num; mem_id++) {
			auto& mem = meta->memberVariables[mem_id];
			member_formated_name = FastNoise::Metadata::FormatMetadataMemberName(mem);

			switch (mem.type) {
				case FastNoise::Metadata::MemberVariable::EFloat:
					ImGui::DragFloat(member_formated_name.c_str(), &data->variables[mem_id].f, mem.valueUiDragSpeed, mem.valueMin.f, mem.valueMax.f);
					break;
				
				case FastNoise::Metadata::MemberVariable::EInt:
					ImGui::DragInt(member_formated_name.c_str(), &data->variables[mem_id].i, mem.valueUiDragSpeed, mem.valueMin.i, mem.valueMax.i);
					break;

				case FastNoise::Metadata::MemberVariable::EEnum:
					ImGui::Combo(member_formated_name.c_str(), &data->variables[mem_id].i, mem.enumNames.begin(), (int)mem.enumNames.size());
					break;
			}

		}
		
		ImGui::PopItemWidth();

		ImNodes::BeginOutputAttribute(node.get_output_attrib_id());
		ImNodes::EndOutputAttribute();

		ImNodes::EndNode();
	}
}

static void draw_output_node() {
	ImNodes::BeginNode(0);
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Output");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(0);
	ImNodes::EndInputAttribute();

	ImNodes::EndNode();
}

static void draw_links() {
	for (auto& [node_id, node] : terr_gen->noise.nodes) {
		int attribute_id = node.get_starting_attrib_id();
		auto links = node.get_lookups();

		for (auto& link : links) {
			if (link) {
				auto& linked_node = terr_gen->noise.nodes.at(link);
				ImNodes::Link(attribute_id, linked_node.get_output_attrib_id(), attribute_id);
			}	
			attribute_id++;
		}
	}

	if (terr_gen->noise.output) {
		auto& link = terr_gen->noise.nodes.at(terr_gen->noise.output);
		ImNodes::Link(0, link.get_output_attrib_id(), 0);
	}
}

static void add_node_popup() {
	if (node_id_to_add != -1) {
		// ImNodes::SetNodeEditorSpacePos(node_id_to_add, position_to_add_new_node);
		ImNodes::SetNodeScreenSpacePos(node_id_to_add, position_to_add_new_node);
		node_id_to_add = -1;
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImNodes::IsEditorHovered()) {
		ImGui::OpenPopup("AddNodePopup");
		position_to_add_new_node = ImGui::GetMousePos();
	}
	
	if (ImGui::BeginPopup("AddNodePopup")) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4,4));
		int n = 0;
		auto all_node_types = FastNoise::Metadata::GetAll();
		for (auto type : all_node_types) {
			const char* temp_name = std::to_string(n++).c_str();
			const char* type_name = FastNoise::Metadata::FormatMetadataNodeName(type).c_str();
			if (ImGui::MenuItem(type_name)) {
				auto new_node = terr_gen->noise.add_node(type);
				node_id_to_add = new_node->id;
				// ImNodes::SetNodeEditorSpacePos(new_node->id, position_to_add_new_node);
			}
		}

		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}
}

static void delete_node(bool del_key_pressed) {
	for (auto& [node_id, node] : terr_gen->noise.nodes) {
		if (ImNodes::IsNodeSelected(node.id) && del_key_pressed) {
			terr_gen->noise.delete_node(node);
		}
	}
}

static void create_links() {
	int start_node_id, end_node_id;
	int start_attr, end_attr;
	bool create_from_snap;

	if (ImNodes::IsLinkCreated(&start_node_id, &start_attr, &end_node_id, &end_attr, &create_from_snap)) {
		noiselib::NoiseGenerator::NoiseNode* start_node = terr_gen->noise.get_node_from_id(start_node_id);
		noiselib::NoiseGenerator::NoiseNode* end_node = terr_gen->noise.get_node_from_id(end_node_id);

		if (start_node && end_node) {
			auto& link = end_node->get_lookup(end_attr);

			if (!create_from_snap || !link) {
				link = start_node->node_data.get();
				APP_LOG_INFO("link created between {} and {}", start_node_id, end_node_id);
			}
		} else if (end_node_id == 0) {
			terr_gen->noise.output = start_node->node_data.get();
		}
	}
}

void app_ui::DrawNoiseNodeEditor() {
	if (!show_noise_editor) return;

	if (ImGui::Begin("Noise Editor", &show_noise_editor)) {
		ImGui::PushItemWidth(100.f);
		ImGui::InputFloat("Noise output axis scale", &terr_gen->noise.scale, 1.f, 10.f);
		ImGui::SameLine();
		ImGui::InputInt("Noise seed", &terr_gen->noise.seed);
		ImGui::PopItemWidth();

		bool del_key_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete), false);

		destroy_links(del_key_pressed);

		ImNodes::BeginNodeEditor();

		draw_nodes();
		draw_output_node();

		draw_links();
		
		add_node_popup();

		ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);

		ImNodes::EndNodeEditor();
		
		try {
			delete_node(del_key_pressed);
		} catch (std::runtime_error &e) {
			APP_LOG_INFO("Error deleting node! {}", e.what());
		}
		
		create_links();

		ImGui::End();
	}
}

void app_ui::DrawMaterialsWindow() {
	if (!show_materials_window) return;

	ImGui::Begin("Terrain Materials", &show_materials_window);

	static int selected = 0;
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("left_pane", ImVec2(150, -25), true);

		for (size_t i = 0; i < terr_gen->materials.data.size(); i++) {
			std::string label = "Material " + std::to_string(i);
			if (ImGui::Selectable(label.c_str(), selected == i)) {
				selected = i;
			}
		}

		ImGui::EndChild();

		if (ImGui::Button("+")) {
			// terrain.materials.push_back(MaterialCreate(2.f, 4.f, 10.f, glm::vec3(0.f, 0.f, 1.f)));
			terr_gen->materials.data.push_back(Material());
		}
		ImGui::SameLine();
		if (ImGui::Button("-")) {
			terr_gen->materials.data.erase(terr_gen->materials.data.begin() + selected);
			selected += (selected > 0) ? -1 : 0;
		}
		ImGui::SameLine();
		if (selected >= 1 && ImGui::Button("<")) {
			std::iter_swap(terr_gen->materials.data.begin()+selected,
						   terr_gen->materials.data.begin()+selected-1);
		}
		ImGui::SameLine();
		if (selected < terr_gen->materials.data.size()-1 && ImGui::Button(">")) {
			std::iter_swap(terr_gen->materials.data.begin()+selected,
						   terr_gen->materials.data.begin()+selected+1);
		}

		ImGui::EndGroup();
	}

	ImGui::SameLine();

	if (terr_gen->materials.data.size() > 0) {
		ImGui::BeginGroup();
		ImGui::PushItemWidth(ImGui::GetFontSize() * 10.f);

		Material* selected_mat = &terr_gen->materials.data.at(selected);

		ImGui::ColorEdit3("Albedo", &selected_mat->albedo.x, ImGuiColorEditFlags_NoInputs);
		// ImGui::ColorEdit3("Diffuse", &selected_mat->diffuse.x, ImGuiColorEditFlags_NoInputs);
		// ImGui::ColorEdit3("Specular", &selected_mat->specular.x, ImGuiColorEditFlags_NoInputs);

		ImGui::Separator();

		ImGui::InputFloat("Metallic", &selected_mat->metallic, 0.01f, 0.1f);

		ImGui::InputFloat("Roughness", &selected_mat->roughness, 0.01f, 0.1f);
		
		ImGui::InputFloat("Blend", &selected_mat->blend, 1.f, 10.f);
		
		ImGui::InputFloat("Max height", &selected_mat->max_height, 0.5f, 1.f);

		ImGui::EndGroup();

		std::clamp(selected_mat->metallic, 0.f, 1.f);
		std::clamp(selected_mat->roughness, 0.f, 1.f);
	}
	
	ImGui::End();
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
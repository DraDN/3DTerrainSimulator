#include "terrain_generator.hpp"
#include "log.hpp"
#include <SDL.h>
#include <stdexcept>

#define HEIGHTS_BUFFER_SHADER_BINDING_BASE 2

TerrainGenerator::TerrainGenerator(glm::uvec2 size, GLenum available_texture_unit) :
		normal_multiplication(1), distance_between_vertecies(1.f), model_size(size),
		normal_map(size.x*normal_multiplication, size.y*normal_multiplication, GL_RGBA32F, GL_RGBA, GL_FLOAT, available_texture_unit, GL_TEXTURE_2D),
		heights(GL_SHADER_STORAGE_BUFFER, false), materials(GL_SHADER_STORAGE_BUFFER, false), model(GL_TRIANGLES) {

	model.bind_callback = [&] {
		if (construct_info.ready_to_upload.load()) {
			GAL_LOG_INFO("Uploading terrain data...");
			model.upload_data();
			calculate_normals();
			// construct_info.ready_to_upload = false;
			construct_info.ready_to_upload.store(false);
		}
		normal_map.bind();
		// materials.upload_data();
		// POTENTIAL PROBLEM - materials.bind() needed(?)
		// materials.bind_base(1);
	};

	model.unbind_callback = [&] {
		// materials.unbind_base(1);
	};

	std::vector<gal::renderer_opengl::VertexAttribute> vert_atts = {
		{ .index = 0,
		  .name = "position",
		  .type = GL_FLOAT,
		  .size = 3},
		{ .index = 1,
		  .name = "aTexCoords",
		  .type = GL_FLOAT,
		  .size = 2}
	};

	model.vertex_buffer.add_vertex_attributes(vert_atts);

	std::vector<gal::renderer_opengl::ShaderInfo> shader_information = {{.path = "./res/normals_shader.comp", .type = GL_COMPUTE_SHADER}};
	normal_map_gen_shader = std::make_unique<gal::renderer_opengl::Shader>(shader_information, std::vector<gal::renderer_opengl::ShaderAttribute>());

	materials.data.emplace_back();

	construct_info.thread_num = std::thread::hardware_concurrency() -1;
	construct_info.reset();

	APP_LOG_INFO("Terrain Generator created...");
}

void TerrainGenerator::generate_test_triangle() {
	model.vertex_buffer.data.push_back(0.f);
	model.vertex_buffer.data.push_back(0.5f);
	model.vertex_buffer.data.push_back(0.f);
	model.vertex_buffer.data.push_back(0.f);
	model.vertex_buffer.data.push_back(0.f);

	model.vertex_buffer.data.push_back(-0.5f);
	model.vertex_buffer.data.push_back(-0.5f);
	model.vertex_buffer.data.push_back(0.f);
	model.vertex_buffer.data.push_back(0.f);
	model.vertex_buffer.data.push_back(0.f);

	model.vertex_buffer.data.push_back(0.5f);
	model.vertex_buffer.data.push_back(-0.5f);
	model.vertex_buffer.data.push_back(0.f);
	model.vertex_buffer.data.push_back(0.f);
	model.vertex_buffer.data.push_back(0.f);

	model.index_buffer.data.push_back(0);
	model.index_buffer.data.push_back(1);
	model.index_buffer.data.push_back(2);
	model.upload_data();
}

void TerrainGenerator::generate() {
	construct_info.reset();
	model.vertex_buffer.data.clear();
	model.index_buffer.data.clear();

	size_t size_vertex_buffer = (model_size.x + 1) * (model_size.y + 1) * model.vertex_buffer.vao->VERTEX_ELEMENT_NUMBER;
	size_t size_index_buffer = model_size.x * model_size.y * 2 * 3;

	model.vertex_buffer.data.resize(size_vertex_buffer);
	model.index_buffer.data.resize(size_index_buffer);
	APP_LOG_INFO("Cleared and resized buffers - model vertex buffer size: {}...", size_vertex_buffer);

	glm::uvec2 normals_size = model_size;
	normals_size += glm::uvec2(1);
	normals_size.x *= normal_multiplication;
	normals_size.y *= normal_multiplication;

	normal_map.update(normals_size.x, normals_size.y);
	heights.data.resize(normals_size.x * normals_size.y);
	APP_LOG_INFO("Updated normal map and height buffer...");

	construct_info.constructing.store(true);

	GAL_LOG_INFO("Launching worker threads...");
	workers = std::move(std::async(std::launch::async, &TerrainGenerator::launch_workers, this));
}

void TerrainGenerator::cancle_generation() {
	construct_info.cancel_generation.store(true);
	GAL_LOG_INFO("Canceling terrain generation...");
}

void TerrainGenerator::launch_workers() {
	std::vector<std::future<void>> builders;

	float axis_scale = 1.f / (float)normal_multiplication;
	noise.get_uniform_grid_2d_scaled(heights.data.data(), (model_size.x +1) * normal_multiplication, (model_size.y +1) * normal_multiplication, axis_scale, axis_scale);

	float start_z = -(float)(model_size.y)/2.f;
	unsigned int length_z = std::floor((model_size.y+1) / construct_info.thread_num);
	int evenly_divider;

	for (uint8_t thr = 0; thr < construct_info.thread_num.load(); thr++) {
		// integer that allocates one more row or not in order to divide uneven numbers across threads
		evenly_divider = (thr < (model_size.y + 1) % construct_info.thread_num.load());

		GAL_LOG_INFO("Builder added...");
		builders.push_back(std::async(std::launch::async, &TerrainGenerator::builder, this, thr, model_size.x, model_size.y, start_z, length_z + evenly_divider, distance_between_vertecies, model.vertex_buffer.vao->VERTEX_ELEMENT_NUMBER, normal_multiplication));
		start_z += length_z + evenly_divider;
	}

	for (auto& builder : builders) {
		builder.wait();
	}

	construct_info.reset();
	construct_info.ready_to_upload.store(true);
	GAL_LOG_INFO("Workers finished...");
}

void TerrainGenerator::builder(int id, unsigned int size_x, unsigned int size_z, float start_z, float work_chunk_size, float distance_between_vertecies, float vertex_size, float normal_mult) {
	glm::vec2 start( -(float)(size_x) / 2.f, start_z);
	glm::vec2 end( -start.x, start_z + work_chunk_size -1.f);

	unsigned int row = start.y + size_z / 2.f;
	//              row where we at |  size of row |
	unsigned int at_vertex     = row * (size_x + 1) * vertex_size;
	unsigned int at_index      = row * (size_x) * 6;
	unsigned int current_index = row * (size_x + 1); 

	GAL_LOG_INFO("Builder {} started...", id);

	for (float z = start.y; z <= end.y; z += 1.f) {
		for (float x = start.x; x <= end.x; x += 1.f) {
			if (construct_info.cancel_generation.load()) {
				GAL_LOG_INFO("Thread {} canceling construction...", id);
				return;
			}
			
			// vertex positions
			try {
				model.vertex_buffer.data.at(at_vertex)     = x * distance_between_vertecies;
				// Shift low-res coordinate from [-size_x/2 .. size_x/2] to [0 .. size_x]
				float shifted_x = x + (size_x / 2.0f);
				float shifted_z = z + (size_z / 2.0f);

				// Scale to high-res grid by normal_mult (integer scale factor)
				int high_x = static_cast<int>(shifted_x * normal_mult);
				int high_z = static_cast<int>(shifted_z * normal_mult);

				// Compute 1D index in high-res height array
				size_t at_heights = high_x + high_z * ((size_x +1) * normal_mult);

				// size_t at_heights = ((x + size_x/2.f) + (z + size_z/2.f) * size_x) * normal_mult;
				model.vertex_buffer.data.at(at_vertex + 1) = heights.data[at_heights];
				model.vertex_buffer.data.at(at_vertex + 2) = z * distance_between_vertecies;

				// vertex texture coordonates
				model.vertex_buffer.data.at(at_vertex + 3) = (x + size_x/2.f) / (size_x); // offset the position to start from 0 and then convert into 0 - 1 range
				model.vertex_buffer.data.at(at_vertex + 4) = (z + size_z/2.f) / (size_z);

				at_vertex += vertex_size;

				// for *model* edge vertecies we don't add triangles
				// size_z is the size of the model on the z and because it's centered, size_z /2.f would be the end of the model
				if (x == end.x || (z == size_z / 2.f))
					continue;

				// triangle 1
				model.index_buffer.data.at(at_index)     = current_index;
				model.index_buffer.data.at(at_index + 1) = current_index + size_x + 1;
				model.index_buffer.data.at(at_index + 2) = current_index + 1;

				// triangle 2
				model.index_buffer.data.at(at_index + 3) = current_index + 1;
				model.index_buffer.data.at(at_index + 4) = current_index + size_x + 1;
				model.index_buffer.data.at(at_index + 5) = current_index + size_x + 2;

				current_index ++;
				at_index += 6;
			} catch (std::out_of_range& e) {
				GAL_LOG_INFO("Something went wrong when asigning data! - {}", e.what());
			}
				
			// construct_info.progress.fetch_add(1);
			// // construct_info.progress.notify_all();
			construct_info.progress++;
		}
		
		// the current index before this increment is the last vertex on the previous line, so this acts as a 'new line increment'
		current_index++;
	}
}

void TerrainGenerator::calculate_normals() {
	GAL_LOG_INFO("Starting normals generator...");
	normal_map_gen_shader->bind();
	normal_map.bind();

	normal_map_gen_shader->set("normals_mult", static_cast<float>(normal_multiplication));
	normal_map_gen_shader->set("distance_between_vertecies", distance_between_vertecies);

	heights.upload_data();
	heights.bind_base(HEIGHTS_BUFFER_SHADER_BINDING_BASE);
	GAL_LOG_INFO("Uploaded heights data...");

	GAL_LOG_INFO("Dispatching compute shader...");
	normal_map_gen_shader->dispatch_compute((model_size.x +1) * normal_multiplication, 1, (model_size.y +1) * normal_multiplication);

	normal_map_gen_shader->unbind();
	normal_map.unbind();
	heights.unbind_base(HEIGHTS_BUFFER_SHADER_BINDING_BASE);
}
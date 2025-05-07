#include <terrain_generator.hpp>
#include <future>
#include <thread>

TerrainGenerator::TerrainGenerator(glm::uvec2 size, GLenum available_texture_unit) :
		normal_multiplication(1),
		normal_map(size.x*normal_multiplication, size.y*normal_multiplication, GL_RGBA32F, GL_RGBA, GL_FLOAT, available_texture_unit, GL_TEXTURE_2D),
		heights(GL_SHADER_STORAGE_BUFFER, false), materials(GL_SHADER_STORAGE_BUFFER, false) {

	model.bind_callback = [&] {
		normal_map.bind();
		materials.upload_data();
		// POTENTIAL PROBLEM - materials.bind() needed(?)
		materials.bind_base(1);
	};

	model.unbind_callback = [&] {
		normal_map.unbind();
		materials.unbind_base(1);
	};

	std::vector<ShaderInfo> shader_information = {{.path = "", .type = GL_COMPUTE_SHADER}};
	normal_map_gen_shader = std::make_unique<gal::renderer_opengl::Shader>(shader_information, std::vector<gal::renderer_opengl::VertexAttribute>());

	materials.data.emplace_back();

	construct_info.thread_num = std::thread::hardware_concurrency();
	construct_info.reset();
}

void TerrainGenerator::generate() {
	construct_info.reset();
	model.vertex_buffer.data.clear();
	model.index_buffer.data.clear();

	size_t size_vertex_buffer = (model_size.x + 1) * (model_size.y + 1) * model.vertex_buffer.vao->VERTEX_SIZE;
	size_t size_index_buffer = model_size.x * model_size.y * 2 * 3;

	model.vertex_buffer.data.resize(size_vertex_buffer);
	model.index_buffer.data.resize(size_index_buffer);

	glm::uvec2 normals_size = model_size;
	normals_size.x *= normal_multiplication;
	normals_size.y *= normal_multiplication;
	normals_size += glm::uvec2(1);

	normal_map.update(normals_size.x, normals_size.y);
	heights.data.resize(normals_size.x * normals_size.y);

	construct_info.constructing = true;

	workers = std::move(std::async(std::launch::async, &TerrainGenerator::launch_workers, this));
}

void TerrainGenerator::cancle_generation() {
	construct_info.cancel_generation = true;
}

void TerrainGenerator::launch_workers() {
	std::vector<std::future<void>> builders;

	noise.get_uniform_grid_2d(heights.data, model_size.x * normal_multiplication, model_size.y * normal_multiplication);

	float start_z = -(float)(model_size.y)/2.f;
	unsigned int length_z = std::floor((model_size.y+1) / construct_info.thread_num);
	int evenly_divider;

	for (uint8_t thr = 0; thr < construct_info.thread_num; thr++) {
		// integer that allocates one more row or not in order to divide uneven numbers across threads
		evenly_divider = (thr < (model_size.y + 1) % construct_info.thread_num);

		builders.push_back(std::async(std::launch::async, &TerrainGenerator::builder, this, thr, model_size.x, model_size.y, start_z, length_z + evenly_divider, distance_between_vertecies, model.vertex_buffer.vao->VERTEX_SIZE, normal_multiplication));
	}

	for (auto& builder : builders) {
		builder.wait();
	}

	calculate_normals();
	model.upload_data();

	// construct_info.constructing = false;
	construct_info.reset();
}

void TerrainGenerator::builder(int id, unsigned int size_x, unsigned int size_z, float start_z, float work_chunk_size, float distance_between_vertecies, float vertex_size, float normal_mult) {
	glm::vec2 start( -(float)(size_x) / 2.f, start_z);
	glm::vec2 end( -start.x, start_z + work_chunk_size -1.f);

	unsigned int row = start.y + size_z / 2.f;
	//          row where we at |  size of row |
	unsigned int at_vertex  = row * (size_x + 1) * vertex_size;
	unsigned int at_index   = row * (size_x    ) * 6;
	unsigned int last_index = row * (size_x + 1); 

	for (float z = start.y; z <= end.y; z += 1.f) {
		for (float x = start.x; x <= end.x; x += 1.f) {
			if (construct_info.cancel_generation)
				return;
			
			// vertex positions
			model.vertex_buffer.data.at(at_vertex)     = x * distance_between_vertecies;
			model.vertex_buffer.data.at(at_vertex + 1) = heights.data[z * normal_mult + x];
			model.vertex_buffer.data.at(at_vertex + 2) = z * distance_between_vertecies;

			// vertex texture coordonates
			model.vertex_buffer.data.at(at_vertex + 3) = (x + size_x/2.f) / (size_x); // offset the position to start from 0 and then convert into 0 - 1 range
			model.vertex_buffer.data.at(at_vertex + 4) = (z + size_z/2.f) / (size_z);

			at_vertex += vertex_size;

			// triangle 1
			model.index_buffer.data.at(at_index)     = last_index;
			model.index_buffer.data.at(at_index + 1) = last_index + size_x + 1;
			model.index_buffer.data.at(at_index + 2) = last_index + 1;

			// triangle 2
			model.index_buffer.data.at(at_index + 3) = last_index + 1;
			model.index_buffer.data.at(at_index + 4) = last_index + size_x + 1;
			model.index_buffer.data.at(at_index + 5) = last_index + size_x + 2;

			last_index ++;
			at_index += 6;
			
			construct_info.progress++;
		}
	}
}

void TerrainGenerator::calculate_normals() {
	normal_map_gen_shader->bind();
	normal_map.bind();

	normal_map_gen_shader->set("normals_mult", static_cast<uint32_t>(normal_multiplication));
	normal_map_gen_shader->set("distance_between_vertecies", distance_between_vertecies);

	heights.upload_data();
	heights.bind_base(0);

	normal_map_gen_shader->dispatch_compute(model_size.x * normal_multiplication, 1, model_size.y * normal_multiplication);

	normal_map_gen_shader->unbind();
	heights.unbind_base(0);
	normal_map.unbind();
}

// void TerrainGenerator::set_size(glm::uvec2 size) {
// 	glm::uvec2 normals_size = size;
// 	normals_size.x *= normal_multiplication;
// 	normals_size.y *= normal_multiplication;
// 	normals_size += glm::uvec2(1);

// 	model_size = size;
// 	normal_map.update(normals_size.x, normals_size.y);
// 	heights.data.resize(normals_size.x * normals_size.y);
// }

// void TerrainGenerator::set_size(float x, float z) {
// 	glm::uvec2 normals_size(x, z);
// 	normals_size.x *= normal_multiplication;
// 	normals_size.y *= normal_multiplication;
// 	normals_size += glm::uvec2(1);

// 	model_size.x = x;
// 	model_size.y = z;
// 	normal_map.update(normals_size.x, normals_size.y);
// 	heights.data.resize(normals_size.x * normals_size.y);
// }
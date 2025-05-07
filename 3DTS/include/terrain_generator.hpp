#pragma once

#include <gal.hpp>
#include <renderer_opengl/model.hpp>
#include <renderer_opengl/texture.hpp>
#include <renderer_opengl/shader.hpp>
#include <noise_generator.hpp>
#include <material.hpp>

#include <vector>
#include <future>

struct MeshConstructInfo {
	std::atomic_bool constructing;
	std::atomic_bool cancel_generation;
	std::atomic_uint progress;

	std::vector<std::future<void>> futures;
	std::atomic_uint8_t thread_num;

	void reset() {
		constructing = false;
		progress = 0.f;
		futures.clear();
	}
};

class TerrainGenerator {
	public:
		TerrainGenerator(glm::uvec2 size, GLenum available_texture_unit);
		void generate();
		void cancle_generation();
		// void set_size(glm::uvec2 size);
		// void set_size(float x, float z);

		gal::renderer_opengl::Model model;
		NoiseGenerator noise;
		gal::renderer_opengl::Texture normal_map;

		gal::renderer_opengl::Buffer<Material> materials;

		MeshConstructInfo construct_info;

		glm::uvec2 model_size;
		float distance_between_vertecies;
		uint8_t normal_multiplication;

	private:
		void launch_workers();
		void builder(int id, unsigned int size_x, unsigned int size_z, float start_z, float work_chunk_size, float distance_between_vertecies, float vertex_size, float normal_mult);
		void calculate_normals();
	
		// glm::uvec2 model_size;
		std::future<void> workers;
		gal::renderer_opengl::Buffer<float> heights;
		std::unique_ptr<gal::renderer_opengl::Shader> normal_map_gen_shader;
};

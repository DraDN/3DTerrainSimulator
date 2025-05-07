#include <noise_generator.hpp>

void NoiseGenerator::get_uniform_grid_2d(std::vector<float>& outputv, float width, float height) {
	output_node->GenUniformGrid2D(outputv.data(), 0, 0, width, height, spacing, seed);
}

float NoiseGenerator::get_single_2d(float x, float y) {
	return output_node->GenSingle2D(x, y, seed);
}

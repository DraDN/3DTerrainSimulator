#include <noise_generator.hpp>

NoiseGenerator::NoiseGenerator(float spacing, int seed) : spacing(spacing), seed(seed) {
	nodes.push_back(FastNoise::New<FastNoise::Perlin>());
}

void NoiseGenerator::get_uniform_grid_2d(AlignedVector<float>& outputv, float width, float height) {
	nodes.back()->GenUniformGrid2D(outputv.data(), 0, 0, width, height, spacing, seed);
}

float NoiseGenerator::get_single_2d(float x, float y) {
	return nodes.back()->GenSingle2D(x, y, seed);
}

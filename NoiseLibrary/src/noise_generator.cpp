#include "noiselib/noise_generator.hpp"
#include "noiselib/log.hpp"

noiselib::NoiseGenerator::NoiseGenerator(float spacing, int seed) : spacing(spacing), seed(seed) {
	nodes.push_back(FastNoise::New<FastNoise::Perlin>());
	NLIB_LOG_INFO("Noise Generator created");
}

void noiselib::NoiseGenerator::get_uniform_grid_2d(noiselib::AlignedVector<float>& outputv, int width, int height) {
	nodes.back()->GenUniformGrid2D(outputv.data(), 0, 0, width, height, seed);
	NLIB_LOG_INFO("Got uniform_grid_2d vector of noise");
}

void noiselib::NoiseGenerator::get_uniform_grid_2d(float* outputv, int width, int height) {
	nodes.back()->GenUniformGrid2D(outputv, 0, 0, width, height, seed);
	NLIB_LOG_INFO("Got uniform_grid_2d vector of noise");
}

float noiselib::NoiseGenerator::get_single_2d(float x, float y) {
	return nodes.back()->GenSingle2D(x, y, seed);
}

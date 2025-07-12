#pragma once

#include <vector>
#include <FastNoise/FastNoise.h>

#include <noiselib/noiselib.hpp>

class noiselib::NoiseGenerator {
	public:
		NoiseGenerator(float spacing = 0.1f, int seed = 1337);

		void get_uniform_grid_2d(noiselib::AlignedVector<float>& outputv, float width, float height);
		float get_single_2d(float x, float y);

		std::vector<FastNoise::SmartNode<>> nodes;

		float spacing;
		int seed;
};
#include <vector>
#include <FastNoise/FastNoise.h>

class NoiseGenerator {
	public:
		NoiseGenerator(float spacing = 0.1f, int seed = 1337) : spacing(spacing), seed(seed), output_node(*new FastNoise::SmartNode<>) {};

		void get_uniform_grid_2d(std::vector<float>& outputv, float width, float height);
		float get_single_2d(float x, float y);

		std::vector<FastNoise::SmartNode<>> nodes;
		FastNoise::SmartNode<>& output_node;

		float spacing;
		int seed;
};
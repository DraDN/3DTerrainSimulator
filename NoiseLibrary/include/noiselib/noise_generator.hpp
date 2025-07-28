#pragma once

#include <vector>
#include <FastNoise/FastNoise.h>
#include <FastNoise/Metadata.h>

#include "noiselib/noiselib.hpp"

class noiselib::NoiseGenerator {
	public:
		NoiseGenerator(float spacing = 0.1f, int seed = 1337);

		void get_uniform_grid_2d(noiselib::AlignedVector<float>& outputv, int width, int height);
		void get_uniform_grid_2d(float* outputv, int width, int height);
		float get_single_2d(float x, float y);

		class NoiseNode {
			public:
				template<typename T>
				static NoiseNode create() {
					NoiseNode node;
					node.noise = FastNoise::New<T>();
					return node;
				}

				FastNoise::SmartNode<> noise;

				template <typename T>
				bool set_property(T property, FastNoise::Metadata::MemberVariable::ValueUnion value) {
					int property_id = resolve_property_id(property);
					if (property_id == -1) return false;
					return _set_property(property_id, value);
				}
			
			private:
				bool _set_property(size_t id, FastNoise::Metadata::MemberVariable::ValueUnion value);
				int resolve_property_id(size_t id);
				int resolve_property_id(const char* name);
		};

		std::vector<NoiseNode> nodes;

		float spacing;
		int seed;
};

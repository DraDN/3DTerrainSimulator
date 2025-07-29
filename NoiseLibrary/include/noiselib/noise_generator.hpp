#pragma once

#include <vector>
#include <FastNoise/FastNoise.h>
#include <FastNoise/Metadata.h>

#include "noiselib/noiselib.hpp"

class noiselib::NoiseGenerator {
	public:
		NoiseGenerator(float scale = 0.1f, int seed = 1337);

		void get_uniform_grid_2d(noiselib::AlignedVector<float>& outputv, int width, int height);
		void get_uniform_grid_2d(float* outputv, int width, int height);
		void get_uniform_grid_2d_scaled(float* outputv, int width, int height, float scaling_x, float scaling_y);
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

				template <typename T>
				FastNoise::Metadata::MemberVariable::ValueUnion get_property(T property) {
					int property_id = resolve_property_id(property);
					if (property_id == -1) return false;
					return _get_property(property_id);
				}

				int get_property_id(const char* name) {
					return resolve_property_id(name);
				}
			
			private:
				bool _set_property(size_t id, FastNoise::Metadata::MemberVariable::ValueUnion value);
				FastNoise::Metadata::MemberVariable::ValueUnion _get_property(size_t id);
				int resolve_property_id(size_t id);
				int resolve_property_id(const char* name);
		};

		std::vector<NoiseNode> nodes;

		float scale;
		int seed;
};

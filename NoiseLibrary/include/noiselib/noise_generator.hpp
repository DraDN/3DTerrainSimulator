#pragma once

#include <vector>
#include <unordered_map>
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
				NoiseNode();

				template<typename T>
				// static NoiseNode create() {
				void create() {
					// NoiseNode node;
					auto n = FastNoise::New<T>();
					// node.node_data = std::make_unique<FastNoise::NodeData>(FastNoise::NodeData(&n->GetMetadata()));
					node_data = std::make_unique<FastNoise::NodeData>(FastNoise::NodeData(&n->GetMetadata()));
					// return node;
				}

				void create(const FastNoise::Metadata* metadata) {
					node_data = std::make_unique<FastNoise::NodeData>(FastNoise::NodeData(metadata));
				}

				std::unique_ptr<FastNoise::NodeData> node_data;
				const int id;

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

				static constexpr int AttributeBitCount = 8;
				static constexpr int AttributeBitMask = (1 << AttributeBitCount) -1;

				static int get_node_id_from_attrib(int attribute_id) {
					return (int)((unsigned int)attribute_id >> AttributeBitCount);
				}

				int get_starting_attrib_id() const {
					return id << AttributeBitCount;
				}

				int get_output_attrib_id() const {
					return get_starting_attrib_id() | AttributeBitMask;
				}

				FastNoise::NodeData*& get_lookup(int lookup_id);
				std::vector<FastNoise::NodeData*> get_lookups();

			private:
				bool _set_property(size_t id, FastNoise::Metadata::MemberVariable::ValueUnion value);
				FastNoise::Metadata::MemberVariable::ValueUnion _get_property(size_t id);
				int resolve_property_id(size_t id);
				int resolve_property_id(const char* name);
			
		};

		NoiseNode* add_node(const FastNoise::Metadata* metadata);
		void delete_node(NoiseNode& node);
		NoiseNode* get_node_from_id(int id);
			
		// std::vector<NoiseNode> nodes;
		std::unordered_map<FastNoise::NodeData*, NoiseNode> nodes;
		FastNoise::NodeData* output;
		// std::unordered_map<int, float> test_map;

		float scale;
		int seed;
};

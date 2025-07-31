#include "noiselib/noise_generator.hpp"
#include "noiselib/log.hpp"

static int last_node_id = 1;

// static FastNoise::SmartNode<> get_noise_generator(const noiselib::NoiseGenerator::NoiseNode* node) {
static FastNoise::SmartNode<> get_noise_generator(FastNoise::NodeData* node_data) {
	// FastNoise::NodeData* data = node->node_data.get();
	std::string serialized = FastNoise::Metadata::SerialiseNodeData(node_data);
	return FastNoise::NewFromEncodedNodeTree(serialized.c_str());
}

noiselib::NoiseGenerator::NoiseGenerator(float scale, int seed) : scale(scale), seed(seed) {
	// nodes.emplace_back(NoiseNode::create<FastNoise::Perlin>());
	// auto start_node = NoiseNode::create<FastNoise::Perlin>();
	NoiseNode start_node;
	start_node.create<FastNoise::Perlin>();
	output = start_node.node_data.get();
	nodes.emplace(std::piecewise_construct, std::forward_as_tuple(start_node.node_data.get()), std::forward_as_tuple(std::move(start_node)));
	NLIB_LOG_INFO("Noise Generator created");
}

noiselib::NoiseGenerator::NoiseNode* noiselib::NoiseGenerator::add_node(const FastNoise::Metadata* metadata) {
	NoiseNode new_node;
	new_node.create(metadata);
	int new_node_id = new_node.id;
	nodes.emplace(std::piecewise_construct, std::forward_as_tuple(new_node.node_data.get()), std::forward_as_tuple(std::move(new_node)));
	return get_node_from_id(new_node_id);
}

void noiselib::NoiseGenerator::delete_node(NoiseNode& node_to_delete) {
	for (auto& node : nodes) {
		// int attrib_id = node.second.get_starting_attrib_id();
		for (auto& link : node.second.get_lookups()) {
			if (link == node_to_delete.node_data.get())
				// node.get_lookup(attrib_id) = nullptr;
				link = nullptr;
			// attrib_id++;
		}
	}

	if (node_to_delete.node_data.get() == output)
		output = nullptr;

	nodes.erase(node_to_delete.node_data.get());
}

noiselib::NoiseGenerator::NoiseNode* noiselib::NoiseGenerator::get_node_from_id(int id) {
	auto find = std::find_if(nodes.begin(), nodes.end(), [id](const auto& node) {
		return node.second.id == id;
	});

	if (find != nodes.end())
		return &find->second;
	
	return nullptr;
}

void noiselib::NoiseGenerator::get_uniform_grid_2d(noiselib::AlignedVector<float>& outputv, int width, int height) {
	if (!output) return;
	auto noise = get_noise_generator(output);
	auto domain_scale = FastNoise::New<FastNoise::DomainScale>();
	domain_scale->SetSource(noise);
	domain_scale->SetScaling(scale);
	domain_scale->GenUniformGrid2D(outputv.data(), 0, 0, width, height, seed);
	NLIB_LOG_INFO("Got uniform_grid_2d vector of noise");
}

void noiselib::NoiseGenerator::get_uniform_grid_2d(float* outputv, int width, int height) {
	if (!output) return;
	auto noise = get_noise_generator(output);
	auto domain_scale = FastNoise::New<FastNoise::DomainScale>();
	domain_scale->SetSource(noise);
	domain_scale->SetScaling(scale);
	domain_scale->GenUniformGrid2D(outputv, 0, 0, width, height, seed);
	NLIB_LOG_INFO("Got uniform_grid_2d vector of noise");
}

void noiselib::NoiseGenerator::get_uniform_grid_2d_scaled(float* outputv, int width, int height, float scaling_x, float scaling_y) {
	if (!output) return;
	auto noise = get_noise_generator(output);
	auto domain_axis_scale = FastNoise::New<FastNoise::DomainAxisScale>();
	domain_axis_scale->SetScaling<FastNoise::Dim::X>(scaling_x * scale);
	domain_axis_scale->SetScaling<FastNoise::Dim::Y>(scaling_y * scale);

	domain_axis_scale->SetSource(noise);

	domain_axis_scale->GenUniformGrid2D(outputv, 0, 0, width, height, seed);
	NLIB_LOG_INFO("Got uniform_grid_2d vector of noise");
}

float noiselib::NoiseGenerator::get_single_2d(float x, float y) {
	if (!output) return 0.f;
	auto noise = get_noise_generator(output);
	return noise->GenSingle2D(x, y, seed);
}

noiselib::NoiseGenerator::NoiseNode::NoiseNode() : id(last_node_id++) {}

bool noiselib::NoiseGenerator::NoiseNode::_set_property(size_t id, FastNoise::Metadata::MemberVariable::ValueUnion value) {
	if (id > node_data->metadata->memberVariables.size()) return false;
	node_data->variables[id] = value;
	return true;
}

FastNoise::Metadata::MemberVariable::ValueUnion noiselib::NoiseGenerator::NoiseNode::_get_property(size_t id) {
	if (id > node_data->metadata->memberVariables.size()) return 0;
	return node_data->variables[id];
}

FastNoise::NodeData*& noiselib::NoiseGenerator::NoiseNode::get_lookup(int lookup_id) {
	lookup_id &= 15;

	if (lookup_id < (int)node_data->nodeLookups.size())
		return node_data->nodeLookups[lookup_id];
	else {
		lookup_id -= (int)node_data->nodeLookups.size();
		return node_data->hybrids[lookup_id].first;
	}
}

std::vector<FastNoise::NodeData*> noiselib::NoiseGenerator::NoiseNode::get_lookups() {
	std::vector<FastNoise::NodeData*> links;
	links.reserve(node_data->nodeLookups.size() + node_data->hybrids.size());

	for (FastNoise::NodeData* link : node_data->nodeLookups)
		links.emplace_back(link);
	
	for (auto& link : node_data->hybrids)
		links.emplace_back(link.first);
	
	return links;
}

int noiselib::NoiseGenerator::NoiseNode::resolve_property_id(size_t id) {
	return id;
}

int noiselib::NoiseGenerator::NoiseNode::resolve_property_id(const char *name) {
	auto& meta = node_data->metadata;
	for (size_t i = 0; i < meta->memberVariables.size(); i++) {
		if (FastNoise::Metadata::FormatMetadataMemberName(meta->memberVariables[i]) == name)
			return i;
	}

	return -1;
}

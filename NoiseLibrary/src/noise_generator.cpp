#include "noiselib/noise_generator.hpp"
#include "noiselib/log.hpp"

noiselib::NoiseGenerator::NoiseGenerator(float scale, int seed) : scale(scale), seed(seed) {
	// nodes.push_back(FastNoise::New<FastNoise::Perlin>());
	auto n = FastNoise::New<FastNoise::Perlin>();
	// auto n_ = FastNoise::NodeData(&n->GetMetadata());
	nodes.emplace_back(NoiseNode::create<FastNoise::Perlin>());
	NLIB_LOG_INFO("Noise Generator created");
}

void noiselib::NoiseGenerator::get_uniform_grid_2d(noiselib::AlignedVector<float>& outputv, int width, int height) {
	auto domain_scale = FastNoise::New<FastNoise::DomainScale>();
	domain_scale->SetSource(nodes.back().noise);
	domain_scale->SetScaling(scale);
	domain_scale->GenUniformGrid2D(outputv.data(), 0, 0, width, height, seed);
	NLIB_LOG_INFO("Got uniform_grid_2d vector of noise");
}

void noiselib::NoiseGenerator::get_uniform_grid_2d(float* outputv, int width, int height) {
	auto domain_scale = FastNoise::New<FastNoise::DomainScale>();
	domain_scale->SetSource(nodes.back().noise);
	domain_scale->SetScaling(scale);
	domain_scale->GenUniformGrid2D(outputv, 0, 0, width, height, seed);
	NLIB_LOG_INFO("Got uniform_grid_2d vector of noise");
}

void noiselib::NoiseGenerator::get_uniform_grid_2d_scaled(float* outputv, int width, int height, float scaling_x, float scaling_y) {
	auto domain_axis_scale = FastNoise::New<FastNoise::DomainAxisScale>();
	domain_axis_scale->SetScaling<FastNoise::Dim::X>(scaling_x * scale);
	domain_axis_scale->SetScaling<FastNoise::Dim::Y>(scaling_y * scale);

	domain_axis_scale->SetSource(nodes.back().noise);

	domain_axis_scale->GenUniformGrid2D(outputv, 0, 0, width, height, seed);
	NLIB_LOG_INFO("Got uniform_grid_2d vector of noise");
}

float noiselib::NoiseGenerator::get_single_2d(float x, float y) {
	return nodes.back().noise->GenSingle2D(x, y, seed);
}

bool noiselib::NoiseGenerator::NoiseNode::_set_property(size_t id, FastNoise::Metadata::MemberVariable::ValueUnion value) {
	auto& meta = noise->GetMetadata();
	if (id > meta.memberVariables.size()) return false;
	meta.memberVariables[id].setFunc(noise.get(), value);
	return true;
}

FastNoise::Metadata::MemberVariable::ValueUnion noiselib::NoiseGenerator::NoiseNode::_get_property(size_t id) {
	auto& meta = noise->GetMetadata();
	auto node_data = FastNoise::NodeData(&meta);
	if (id > meta.memberVariables.size()) return 0;
	return node_data.variables[id];
}

int noiselib::NoiseGenerator::NoiseNode::resolve_property_id(size_t id) {
	return id;
}

int noiselib::NoiseGenerator::NoiseNode::resolve_property_id(const char *name) {
	auto& meta = noise->GetMetadata();
	for (size_t i = 0; i < meta.memberVariables.size(); i++) {
		if (FastNoise::Metadata::FormatMetadataMemberName(meta.memberVariables[i]) == name)
			return i;
	}

	return -1;
}

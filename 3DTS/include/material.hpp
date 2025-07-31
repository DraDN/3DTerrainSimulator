#pragma once

#include "gal/glm_tools.hpp"

struct Material {
	float max_height = 100.f;
	float max_steepness = 1.f;
	float blend = 10.f;

	alignas(16) glm::vec3 albedo = glm::vec3(1.f);
	float metallic = 0.f;
	float roughness = 1.f;
	float ao = 1.f;
};

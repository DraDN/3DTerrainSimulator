#pragma once

#include <glm_tools.hpp>

struct Material {
	float height = 100.f;
	float blend = 10.f;

	glm::vec3 albedo = glm::vec3(1.f);
	float metallic = 0.f;
	float roughness = 1.f;
	float ao = 1.f;
};

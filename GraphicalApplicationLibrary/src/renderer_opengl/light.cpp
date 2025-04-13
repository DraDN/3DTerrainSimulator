#include <renderer_opengl/light.hpp>
#include <renderer_opengl/shader.hpp>

void gal::renderer_opengl::Light::send_shader_uniforms(Shader& shader) {
	shader.bind();
	shader.set("light.position", position);
	shader.set("light.color", color);
	shader.set("light.ambient", ambient_intensity);
	shader.set("light.diffuse", diffuse_intensity);
	shader.set("light.specular", specular_intensity);
	shader.set("light.projection", camera.get_projection());
	shader.set("light.view", camera.get_view());
}

void gal::renderer_opengl::Light::rotate_around_point_by(glm::vec3 point, float angle) {
	auto t1 = glm::translate(glm::mat4(1.f), -point);
	auto r  = glm::rotate(glm::mat4(1.f), glm::degrees(angle), glm::vec3(0.f, 1.f, 0.f));
	auto t2 = glm::translate(glm::mat4(1.f),  point);

	position = (t1 * r * t2) * glm::translate(glm::mat4(1.f), position) * glm::vec4(0.f, 0.f, 0.f, 1.f);
}

void gal::renderer_opengl::Light::set_position(glm::vec3 new_pos) {
	position = new_pos;
	camera.set_position(new_pos);
}

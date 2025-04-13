#pragma once

#include <gal.hpp>
#include <renderer_opengl/camera.hpp>
#include <renderer_opengl/render_target.hpp>
#include <glm_tools.hpp>

class gal::renderer_opengl::Light {
	public:
		// Light();
		Light(GLenum depth_texture_unit,
			  glm::vec3 pos = glm::vec3(1.f),
			  glm::vec2 size = glm::vec3(100.f),
			  glm::vec3 color = glm::vec3(1.f),
			  float aI = 1.f,
			  float dI = 1.f,
			  float sI = 1.f) : 
			  	position(pos), color(color),
				ambient_intensity(aI), diffuse_intensity(dI), specular_intensity(sI),
				camera(size, pos, glm::vec3(0.f)),
				render_target(GL_DEPTH_ATTACHMENT, glm::uvec2(depth_map_resolution), GL_RGBA16, GL_RGBA, GL_FLOAT, depth_texture_unit) {};

		void send_shader_uniforms(Shader& shader);

		// void set_projection_size(float size_x, float size_y);
		// void calculate_matricies();
		void rotate_around_point_by(glm::vec3 point, float angle);
		void set_position(glm::vec3 new_pos);

		glm::vec3 color;
		float ambient_intensity, diffuse_intensity, specular_intensity;
		
		// bool use_shadow_offset = true;
		RenderTarget render_target;
		
	private:
		glm::vec3 position;
		Camera camera;

		const uint16_t depth_map_resolution = 1024 * 4;
};

#pragma once

#include <gal.hpp>
// #include <texture.hpp>
#include <glm_tools.hpp>

#define NEAR 0.1f
#define FAR 5000.f

class gal::renderer_opengl::Camera {
	public:
		Camera(glm::vec2 proj_size, glm::vec3 pos, glm::vec3 look_at, float fov = 0.f);

		void send_shader_uniforms(Shader& shader);

		void set_size(float width, float height);

		// void rotate(float angle, glm::vec3 axis);
		void rotate(float yaw, float pitch);
		void rotate_by(float d_angle, glm::vec3 axis);
		virtual void rotate_by(float d_yaw, float d_pitch);

		void zoom(float zoom);

		void set_position(glm::vec3 new_pos);
		virtual void move_by(glm::vec3 d_pos);

		glm::mat4 get_projection();
		glm::mat4 get_view();
	protected:
		glm::mat4 projection;
		glm::mat4 view;
		
		float fov;
		glm::vec2 size;
		float yaw, pitch;
		glm::vec3 position;
		glm::vec3 front, up;
		
		bool is_ortho;
		void _calculate_projection();
		float _angle_between(glm::vec3 a, glm::vec3 b, glm::vec3 origin, bool acos_or_asin);
};

class gal::renderer_opengl::ControllableCamera : Camera {
	public:
		using Camera::Camera;

		void rotate_by(float d_yaw, float d_pitch);
		void move_by(glm::vec3 d_pos);

		float look_sensitivity = 1.f;
		float move_speed = 1.f;
};

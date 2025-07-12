#pragma once

#include "gal/gal.hpp"
#include "gal/glm_tools.hpp"
#include <GL/glew.h>

#include "gal/renderer_opengl/vertex_attribute.hpp"

#include <string>
#include <vector>

struct gal::renderer_opengl::ShaderInfo {
	std::string path;
	GLenum type;
};

struct gal::renderer_opengl::ShaderAttribute {
	GLuint index;
	std::string name;
};

class gal::renderer_opengl::Shader {
	public:
		GLuint handle;

		Shader(std::string vs_path, std::string fs_path, size_t n, std::vector<ShaderAttribute> attributes);
		Shader(std::vector<ShaderInfo> shader_information, std::vector<ShaderAttribute> attributes);
		~Shader();

		void bind();
		void unbind();

		void dispatch_compute(uint16_t comp_size_x, uint16_t comp_size_y, uint16_t comp_size_z);

		void set(std::string name, glm::mat4 m);
		void set(std::string name, glm::vec4 f4);
		void set(std::string name, glm::vec3 f3);
		void set(std::string name, float f);
		void set(std::string name, uint32_t u);

	private:
		bool is_compute;
		static GLuint bound_shader_handle;
};

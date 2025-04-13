#pragma once

#include <gal.hpp>
#include <glm_tools.hpp>
#include <GL/glew.h>

#include <renderer_opengl/vertex_attribute.hpp>

#include <string>
#include <vector>

struct ShaderInfo {
	std::string path;
	GLenum type;
};

class gal::renderer_opengl::Shader {
	public:
		GLuint handle;

		// Shader(const char* vs_path, const char* fs_path, size_t n, VertexAttribute attributes[]);
		Shader(std::string vs_path, std::string fs_path, size_t n, std::vector<VertexAttribute> attributes);
		// Shader(const char* comp_s_path);
		// Shader(std::string comp_s_path);
		Shader(std::vector<ShaderInfo> shader_information, std::vector<VertexAttribute> uniforms);
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

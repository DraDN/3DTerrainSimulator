#pragma once

#include <gal.hpp>
#include <GL/glew.h>
#include <glm_tools.hpp>
#include <vector>

// typedef std::vector<std::pair<GLenum, GLenum>> ParamterList;

class gal::renderer_opengl::Texture {
	public:
		GLuint handle;
		glm::uvec2 size;
		GLenum unit, target;

		// Texture(uint16_t size_x_, uint16_t size_y_, GLenum internal_format, GLenum format, GLenum type, uint16_t tex_unit, uint16_t tex_target, ParamterList paramters);
		Texture(uint16_t size_x_, uint16_t size_y_, GLenum internal_format, GLenum format, GLenum type, GLenum tex_unit, GLenum tex_target);
		~Texture();

		void bind();
		void unbind();
		void update(uint16_t size_x_, uint16_t size_y_);
		void update(void* pixels);
		void update(uint16_t size_x_, uint16_t size_y_, void* pixels);

	private:
		GLenum internal_format, format, type;
		// ParamterList default_parameters_image;
		// ParamterList default_parameters_depth;
};

#pragma once

#include "gal/gal.hpp"
#include "gal/renderer_opengl/texture.hpp"

#include <GL/glew.h>
#include "gal/glm_tools.hpp"

class gal::renderer_opengl::RenderTarget {
	public:
		// TODO: add render buffer for depth testing etc.
		RenderTarget(GLenum attachment, glm::uvec2 size, GLenum texture_internal_format, GLenum texture_format, GLenum texture_type, GLenum tex_unit);
		~RenderTarget();

		void bind();
		static void bind_window_target(glm::uvec2 size);

		GLuint handle;
		GLenum attachment;
		Texture output;
};

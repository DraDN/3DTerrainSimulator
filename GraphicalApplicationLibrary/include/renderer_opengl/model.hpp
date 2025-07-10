#pragma once

#include <gal.hpp>
#include <glm_tools.hpp>
#include <renderer_opengl/buffer.hpp>
#include <renderer_opengl/vertex_attribute.hpp>
#include <vector>
#include <GL/glew.h>
#include <functional>

class gal::renderer_opengl::Model {
	public:
		Model(GLenum render_mode = GL_TRIANGLES, bool dynamic_buffers = false);
		Model(std::vector<VertexAttribute>& vertex_attributes, GLenum render_mode = GL_TRIANGLES, bool dynamic_buffers = false);

		void add_vertex_attributes(std::vector<VertexAttribute>& vertex_attributes);
		void upload_data();
		void render();
		
		GLenum render_mode;

		glm::mat4 transform_matrix;

		Buffer<GLfloat> vertex_buffer;
		Buffer<GLuint> index_buffer;
		std::function<void()> bind_callback;
		std::function<void()> unbind_callback;
};

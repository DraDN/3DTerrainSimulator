#pragma once

#include <gal.hpp>
#include <glm_tools.hpp>
#include <renderer_opengl/buffer.hpp>
#include <renderer_opengl/vertex_attribute.hpp>
#include <vector>
#include <GL/glew.h>

class gal::renderer_opengl::Model {
	public:
		Model(GLenum render_mode = GL_TRIANGLES, bool dynamic_buffers = false);
		Model(std::vector<VertexAttribute>& vertex_attributes, GLenum render_mode = GL_TRIANGLES, bool dynamic_buffers = false);
		// ~Model();

		void add_vertex_attributes(std::vector<VertexAttribute>& vertex_attributes);
		void upload_data();
		void render();
		
		GLenum render_mode;

		glm::mat4 transform_matrix;

		Buffer<float> vertex_buffer;
		Buffer<uint16_t> index_buffer;
};

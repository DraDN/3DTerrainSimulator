#pragma once

#include "gal/gal.hpp"
#include "gal/renderer_opengl/vao.hpp"
#include "gal/renderer_opengl/vertex_attribute.hpp"
#include <optional>
#include <vector>

template <typename T> 
class gal::renderer_opengl::Buffer {
	public:
		GLuint handle;
		GLint type;
		GLboolean dynamic;

		std::vector<T> data;

		std::optional<VAO> vao;

		Buffer(GLint type, GLboolean dynamic);
		~Buffer();

		void bind();
		void bind_vao();
		void unbind();
		void unbind_vao();
		void add_vertex_attributes(std::vector<VertexAttribute>& vertex_attributes);
		void upload_data();
		void bind_base(GLuint base);
		void unbind_base(GLuint base);
};

#include "gal/renderer_opengl/buffer.tpp"

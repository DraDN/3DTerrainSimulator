#pragma once

#include <gal.hpp>
#include <renderer_opengl/vao.hpp>
#include <renderer_opengl/vertex_attribute.hpp>
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

		Buffer(GLint type, GLboolean dynamic) : type(type), dynamic(dynamic);
		~Buffer();

		void bind();
		void unbind();
		// void bind_data(void* data, size_t offset, size_t count);
		void add_vertex_attributes(std::vector<VertexAttribute>& vertex_attributes);
		void upload_data();
		void bind_base(GLuint base);
		void unbind_base(GLuint base);
};

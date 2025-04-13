#pragma once

#include <gal.hpp>
#include <renderer_opengl/vertex_attribute.hpp>

#include <vector>

class gal::renderer_opengl::VAO {
	public:
		VAO();
		~VAO();
		
		void bind();
		void unbind();
		template <typename T>
		void add_attributes(Buffer<T>& buffer, size_t n, VertexAttribute attributes[]);
		template <typename T>
		void add_attributes(Buffer<T>& buffer, std::vector<VertexAttribute> attributes);

		GLuint handle;
};

#include "gal/renderer_opengl/vao.hpp"

gal::renderer_opengl::VAO::VAO() {
	glGenVertexArrays(1, &handle);
	VERTEX_ELEMENT_NUMBER = 0;
	VERTEX_SIZE = 0;
}

gal::renderer_opengl::VAO::~VAO() {
	glDeleteVertexArrays(1, &handle);
}

void gal::renderer_opengl::VAO::bind() {
	glBindVertexArray(handle);
}

void gal::renderer_opengl::VAO::unbind() {
	glBindVertexArray(0);
}
#include <renderer_opengl/vao.hpp>

gal::renderer_opengl::VAO::VAO() {
	glGenVertexArrays(1, &handle);
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

static void _vao_add_attribute(GLuint index, GLuint size, GLenum type, GLsizei stride, size_t offset) {
	glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)offset);
	glEnableVertexAttribArray(index);
}

template <typename T>
void gal::renderer_opengl::VAO::add_attributes(Buffer<T>& buffer, size_t n, gal::renderer_opengl::VertexAttribute attributes[]) {
	bind();
	buffer.bind();

	size_t offset = 0;
	GLsizei stride = 0;

	for (size_t i = 0; i < n; i++)
		stride += attributes[i].size * sizeof(attributes[i].type);
	
	for (size_t i = 0; i < n; i++) {
		_vao_add_attribute(attributes[i].index, attributes[i].size, attributes[i].type, stride, offset);
		offset += attributes[i].size * sizeof(attributes[i].type);
	}

	unbind();
	buffer.unbind();
}

template <typename T>
void gal::renderer_opengl::VAO::add_attributes(Buffer<T>& buffer, std::vector<VertexAttribute>& attributes) {
	bind();
	buffer.bind();

	size_t offset = 0;
	GLsizei stride = 0;

	for (auto& att : attributes)
		stride += att.size * sizeof(att.type);
	
	for (auto& att: attributes) {
		_vao_add_attribute(att.index, att.size, att.type, stride, offset);
		offset += att.size * sizeof(att.type);
	}
}

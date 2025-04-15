#include <renderer_opengl/buffer.hpp>

template <typename T>
gal::renderer_opengl::Buffer<T>::Buffer(GLint type, GLboolean dynamic) : type(type), dynamic(dynamic) {
	glGenBuffers(1, &handle);

	if (type == GL_ARRAY_BUFFER) {
		vao.value() = VAO();
	}
}

template <typename T>
gal::renderer_opengl::Buffer<T>::~Buffer() {
	glDeleteBuffers(1, handle);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::bind() {
	if (vao.has_value()) vao->bind();
	glBindBuffer(type, handle);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::unbind() {
	if (vao.has_value()) vao->unbind();
	glBindBuffer(type, 0);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::add_vertex_attributes(std::vector<VertexAttribute>& vertex_attributes) {
	if (!vao.has_value()) return;
	vao->add_attributes(this, &vertex_attributes);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::upload_data() {
	bind();
	glBufferData(type, data.size(), data.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::bind_base(GLuint base) {
	glBindBufferBase(type, base, handle);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::unbind_base(GLuint base) {
	glBindBufferBase(type, base, 0);
}

#include "gal/renderer_opengl/model.hpp"

gal::renderer_opengl::Model::Model(GLenum render_mode, bool dynamic_buffers) : vertex_buffer(GL_ARRAY_BUFFER, dynamic_buffers), index_buffer(GL_ELEMENT_ARRAY_BUFFER, dynamic_buffers) {
	transform_matrix = glm::mat4(1.f);
	this->render_mode = render_mode;
}

gal::renderer_opengl::Model::Model(std::vector<VertexAttribute>& vertex_attributes, GLenum render_mode, bool dynamic_buffers) :
 vertex_buffer(GL_ARRAY_BUFFER, dynamic_buffers), index_buffer(GL_ELEMENT_ARRAY_BUFFER, dynamic_buffers) {
	transform_matrix = glm::mat4(1.f);
	vertex_buffer.add_vertex_attributes(vertex_attributes);
	this->render_mode = render_mode;
}

void gal::renderer_opengl::Model::add_vertex_attributes(std::vector<VertexAttribute>& vertex_attributes) {
	vertex_buffer.add_vertex_attributes(vertex_attributes);
}

void gal::renderer_opengl::Model::upload_data() {
	vertex_buffer.upload_data();
	index_buffer.upload_data();
}

void gal::renderer_opengl::Model::render() {
	vertex_buffer.bind_vao();
	index_buffer.bind();
	if (bind_callback)
		bind_callback();
	glDrawElements(render_mode, index_buffer.data.size(), GL_UNSIGNED_INT, 0);
	if (unbind_callback)
		unbind_callback();
	vertex_buffer.unbind_vao();
	index_buffer.unbind();
}

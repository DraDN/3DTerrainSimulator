#include <renderer_opengl/model.hpp>

gal::renderer_opengl::Model::Model(GLenum render_mode, bool dynamic_buffers) : vertex_buffer(GL_ARRAY_BUFFER, dynamic_buffers), index_buffer(GL_ELEMENT_ARRAY_BUFFER, dynamic_buffers) {
	transform_matrix = glm::mat4(1.f);
}

gal::renderer_opengl::Model::Model(std::vector<VertexAttribute>& vertex_attributes, GLenum render_mode, bool dynamic_buffers) :
 vertex_buffer(GL_ARRAY_BUFFER, dynamic_buffers), index_buffer(GL_ELEMENT_ARRAY_BUFFER, dynamic_buffers) {
	transform_matrix = glm::mat4(1.f);
	vertex_buffer.add_vertex_attributes(vertex_attributes);
}

void gal::renderer_opengl::Model::add_vertex_attributes(std::vector<VertexAttribute>& vertex_attributes) {
	vertex_buffer.add_vertex_attributes(vertex_attributes);
}

void gal::renderer_opengl::Model::upload_data() {
	vertex_buffer.upload_data();
	index_buffer.upload_data();
}

void gal::renderer_opengl::Model::render() {
	vertex_buffer.bind();
	index_buffer.bind();
	glDrawElements(render_mode, index_buffer.data.size(), index_buffer.type, 0);
	vertex_buffer.unbind();
	index_buffer.unbind();
}

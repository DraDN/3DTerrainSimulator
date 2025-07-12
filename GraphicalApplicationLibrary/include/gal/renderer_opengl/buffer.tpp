#pragma once

#include <SDL.h>
#include <stdexcept>

template <typename T>
gal::renderer_opengl::Buffer<T>::Buffer(GLint type, GLboolean dynamic) : type(type), dynamic(dynamic), vao(std::nullopt) {
    glGenBuffers(1, &handle);

    if (type == GL_ARRAY_BUFFER) {
        vao.emplace();
    }
}

template <typename T>
gal::renderer_opengl::Buffer<T>::~Buffer() {
    glDeleteBuffers(1, &handle);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::bind() {
    glBindBuffer(type, handle);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::bind_vao() {
    if (!vao.has_value()) return;
    vao->bind();
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::unbind() {
    glBindBuffer(type, 0);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::unbind_vao() {
    if (!vao.has_value()) return;
    vao->unbind();
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::add_vertex_attributes(std::vector<VertexAttribute>& vertex_attributes) {
    if (!vao.has_value()) return;
    vao->add_attributes(*this, vertex_attributes);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::upload_data() {
    bind();
    glBufferData(type, data.size() * sizeof(T), data.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::bind_base(GLuint base) {
    glBindBufferBase(type, base, handle);
}

template <typename T>
void gal::renderer_opengl::Buffer<T>::unbind_base(GLuint base) {
    glBindBufferBase(type, base, 0);
}

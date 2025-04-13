#include <renderer_opengl/texture.hpp>

gal::renderer_opengl::Texture::Texture(uint16_t size_x_, uint16_t size_y_, GLenum internal_format, GLenum format, GLenum type, GLenum tex_unit, GLenum tex_target) {
	size.x = size_x_;
	size.y = size_y_;

	unit = tex_unit;
	target = tex_target;

	this->internal_format = internal_format;
	this->format = format;
	this->type = type;

	glGenTextures(1, &handle);

	glActiveTexture(unit);
	glBindTexture(target, handle);

	glTexImage2D(target, 0, internal_format, size.x, size.y, 0, format, type, nullptr);

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (internal_format == GL_DEPTH_COMPONENT) {
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, border_color);
	} else {
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindImageTexture(0, handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, internal_format);
	}

	glActiveTexture(0);
}

gal::renderer_opengl::Texture::~Texture() {
	glDeleteTextures(1, &handle);
}

void gal::renderer_opengl::Texture::bind() {
	glActiveTexture(unit);
	glBindTexture(target, handle);
}

void gal::renderer_opengl::Texture::unbind() {
	glActiveTexture(0);
	glBindTexture(target, 0);
}

void gal::renderer_opengl::Texture::update(uint16_t size_x_, uint16_t size_y_) {
	size.x = size_x_;
	size.y = size_y_;

	bind();
	glTexImage2D(target, 0, internal_format, size.x, size.y, 0, format, type, nullptr);
	unbind();
}

void gal::renderer_opengl::Texture::update(void* pixels) {
	bind();
	glTexImage2D(target, 0, internal_format, size.x, size.y, 0, format, type, pixels);
	unbind();
}

void gal::renderer_opengl::Texture::update(uint16_t size_x_, uint16_t size_y_, void* pixels) {
	size.x = size_x_;
	size.y = size_y_;

	bind();
	glTexImage2D(target, 0, internal_format, size.x, size.y, 0, format, type, pixels);
	unbind();
}

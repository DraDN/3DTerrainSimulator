#include <renderer_opengl/render_target.hpp>

gal::renderer_opengl::RenderTarget::RenderTarget(GLenum attachment, glm::uvec2 size, GLenum texture_internal_format, GLenum texture_format, GLenum texture_type, GLenum tex_unit) : 
		output(size.x, size.y, texture_internal_format, texture_format, texture_type, tex_unit, GL_TEXTURE_2D) {
	output.bind();
	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, output.handle, 0);

	if (attachment == GL_DEPTH_ATTACHMENT) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	output.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

gal::renderer_opengl::RenderTarget::~RenderTarget() {
	glDeleteFramebuffers(1, &handle);
}

void gal::renderer_opengl::RenderTarget::bind() {
	glViewport(0, 0, output.size.x, output.size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

// void gal::renderer_opengl::RenderTarget::unbind() {
void gal::renderer_opengl::RenderTarget::bind_window_target(glm::uvec2 size) {
	glViewport(0, 0, size.x, size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

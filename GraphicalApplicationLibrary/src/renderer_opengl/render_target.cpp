#include "gal/renderer_opengl/render_target.hpp"

gal::renderer_opengl::RenderTarget::RenderTarget(GLenum attachment, glm::uvec2 size, GLenum texture_internal_format, GLenum texture_format, GLenum texture_type, GLenum tex_unit) : 
		output(size.x, size.y, texture_internal_format, texture_format, texture_type, tex_unit, GL_TEXTURE_2D), attachment(attachment) {
	output.bind();
	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, output.handle, 0);

	if (attachment == GL_DEPTH_ATTACHMENT) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	} else {
		glGenRenderbuffers(1, &rbo_handle);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_handle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_handle);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		GAL_LOG_ERROR("Framebuffer is not complete!");

	output.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

gal::renderer_opengl::RenderTarget::~RenderTarget() {
	glDeleteRenderbuffers(1, &rbo_handle);
	glDeleteFramebuffers(1, &handle);
}

void gal::renderer_opengl::RenderTarget::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glViewport(0, 0, output.size.x, output.size.y);
}

void gal::renderer_opengl::RenderTarget::bind_window_target(glm::uvec2 size) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, size.x, size.y);
}

void gal::renderer_opengl::RenderTarget::resize(glm::uvec2 size) {
	output.update(size.x, size.y);
	
	glDeleteRenderbuffers(1, &rbo_handle);
	glGenRenderbuffers(1, &rbo_handle);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_handle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);

	glBindFramebuffer(GL_FRAMEBUFFER, handle);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_handle);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

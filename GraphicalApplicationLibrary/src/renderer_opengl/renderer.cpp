#include <renderer_opengl/renderer.hpp>
#include <renderer_opengl/model.hpp>
#include <renderer_opengl/light.hpp>

void gal::renderer_opengl::RendererOpenGL::render(Model& model, Shader& shader, Camera& camera, std::optional<RenderTarget&> render_target = {}, std::optional<Light&> light) {
	camera.send_shader_uniforms(shader);

	if (light.has_value())
		light.value().send_shader_uniforms(shader);
	
	if (render_target.has_value())
		render_target.value().bind();
	
	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	model.render();

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (render_target.has_value())
		render_target.value().bind_window_target(window_size);

	shader.unbind();
}

void gal::renderer_opengl::RendererOpenGL::clear() {
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
}

#include "gal/renderer_opengl/renderer.hpp"
#include "gal/renderer_opengl/model.hpp"
#include "gal/renderer_opengl/light.hpp"

void gal::renderer_opengl::RendererOpenGL::render(Model& model, Shader& shader, Camera& camera, std::optional<std::reference_wrapper<RenderTarget>> render_target, std::optional<std::reference_wrapper<Light>> light) {
	camera.send_shader_uniforms(shader); // auto binds the shader

	if (light.has_value())
		light->get().send_shader_uniforms(shader);
	
	if (render_target.has_value())
		render_target->get().bind();
	else
		RenderTarget::bind_window_target(window_size);
	
	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shader.set("model", model.transform_matrix);
	model.render();

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (render_target.has_value())
		render_target->get().bind_window_target(window_size);

	shader.unbind();
}

void gal::renderer_opengl::RendererOpenGL::clear(std::optional<std::reference_wrapper<RenderTarget>> render_target) {
	if (render_target.has_value())
		render_target->get().bind();

	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

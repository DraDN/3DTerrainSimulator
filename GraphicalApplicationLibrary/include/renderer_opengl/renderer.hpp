#pragma once

#include <gal.hpp>
#include <renderer_opengl/shader.hpp>
#include <renderer_opengl/camera.hpp>

#include <glm_tools.hpp>
#include <SDL.h>
#include <optional>

class gal::renderer_opengl::RendererOpenGL {
	public:
		RendererOpenGL(glm::uvec2 window_size) : window_size(window_size) {};
		// ~RendererOpenGL();
		
		// TODO: exchange model for a "scene" class, a container of multiple models
		// void render(Model& model, const std::optional<Camera>& camera = {}, const std::optional<Light>& light = {}, const std::optional<Shader>& shader = {});
		void render(Model& model, Shader& shader, Camera& camera, std::optional<RenderTarget>* render_target = nullptr, std::optional<Light>* light = nullptr);
		// void render_to(Model& model, Shader& shader, Camera& camera, RenderTarget& render_target, std::optional<Light&> light = {});
		void clear();
		
		glm::vec4 clear_color = glm::vec4(0.f);
		glm::uvec2 window_size;
		bool wireframe = false;
	// private:
		// Shader def_shader;
		// Camera def_camera;
};

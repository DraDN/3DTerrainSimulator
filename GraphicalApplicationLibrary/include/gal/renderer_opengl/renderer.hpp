#pragma once

#include "gal/gal.hpp"
#include "gal/renderer_opengl/shader.hpp"
#include "gal/renderer_opengl/camera.hpp"
#include "gal/renderer_opengl/render_target.hpp"

#include "gal/glm_tools.hpp"
#include <SDL.h>
#include <optional>
#include <functional>

class gal::renderer_opengl::RendererOpenGL {
	public:
		RendererOpenGL(glm::uvec2 window_size) : window_size(window_size) {};
		
		// TODO: exchange model for a "scene" class, a container of multiple models
		// void render(Model& model, const std::optional<Camera>& camera = {}, const std::optional<Light>& light = {}, const std::optional<Shader>& shader = {});
		void render(Model& model, Shader& shader, Camera& camera, std::optional<std::reference_wrapper<RenderTarget>> render_target = std::nullopt, std::optional<std::reference_wrapper<Light>> light = std::nullopt);
		void clear(std::optional<std::reference_wrapper<RenderTarget>> render_target = std::nullopt);
		
		glm::vec4 clear_color = glm::vec4(0.f, 0.f, 0.f, 1.f);
		glm::uvec2 window_size;
		bool wireframe = false;
};

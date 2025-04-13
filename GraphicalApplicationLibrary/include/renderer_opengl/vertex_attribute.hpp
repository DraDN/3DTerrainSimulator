#pragma once

#include <gal.hpp>
#include <GL/glew.h>

struct gal::renderer_opengl::VertexAttribute {
	GLuint index;
	const GLchar *name;
	const GLenum type;
	const GLuint size;
};

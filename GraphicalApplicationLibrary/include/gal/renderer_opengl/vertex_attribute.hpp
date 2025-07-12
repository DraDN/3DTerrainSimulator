#pragma once

#include "gal/gal.hpp"
#include <GL/glew.h>

/*
* @param index - the index of the attribute
* @param *name - the name of the attribute 
* @param type - the data type of the attribute
* @param size - the number of type the attribute is made out of
*/
struct gal::renderer_opengl::VertexAttribute {
	GLuint index;
	const GLchar *name;
	const GLenum type;
	const GLuint size;
};

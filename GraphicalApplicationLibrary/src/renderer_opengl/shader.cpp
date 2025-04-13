#include <renderer_opengl/shader.hpp>

#include <renderer_opengl/vertex_attribute.hpp>

#include <fstream>
#include <sstream>
#include <unordered_map>

static std::unordered_map<std::string, GLint> uniform_location_cache;
GLuint gal::renderer_opengl::Shader::bound_shader_handle = 0;

static GLint _compile_shader(std::string path, GLenum type) {
	std::ifstream shader_file;
	std::string shader_code_cpy;

	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shader_file.open(path);

		std::stringstream string_stream;
		string_stream << shader_file.rdbuf();

		shader_file.close();

		shader_code_cpy = string_stream.str();
	} catch(std::ifstream::failure e) {
		exit(1);
	}

	const char* shader_code = shader_code_cpy.c_str();
	
	GLuint shader_handler = glCreateShader(type);
	glShaderSource(shader_handler, 1, &shader_code, NULL);
	glCompileShader(shader_handler);

	GLint success;
	glGetShaderiv(shader_handler, GL_COMPILE_STATUS, &success);
	if (!success) {
		char buf[512];
		glGetShaderInfoLog(shader_handler, 512, NULL, buf);
		exit(1);
	}

	return shader_handler;
}

gal::renderer_opengl::Shader::Shader(std::string vs_path, std::string fs_path, size_t n, std::vector<VertexAttribute> attributes) {
	GLuint vs_handle = _compile_shader(vs_path, GL_VERTEX_SHADER);
	GLuint fs_handle = _compile_shader(fs_path, GL_FRAGMENT_SHADER);

	handle = glCreateProgram();
	glAttachShader(handle, vs_handle);
	glAttachShader(handle, fs_handle);

	for (auto& att : attributes)
		glBindAttribLocation(handle, att.index, att.name);

	glLinkProgram(handle);

	GLint linked;
	glGetProgramiv(handle, GL_LINK_STATUS, &linked);

	if (!linked) {
		char buf[512];
		glGetProgramInfoLog(handle, 512, NULL, buf);
		exit(0);
	}

	glDeleteShader(vs_handle);
	glDeleteShader(fs_handle);
}

gal::renderer_opengl::Shader::Shader(std::vector<ShaderInfo> shader_information, std::vector<VertexAttribute> uniforms) {
	handle = glCreateProgram();

	std::vector<GLuint> shader_handles;
	for (auto& shader : shader_information) {
		is_compute = (shader.type == GL_COMPUTE_SHADER) ? true : false;

		shader_handles.push_back(_compile_shader(shader.path, shader.type));
		glAttachShader(handle, shader_handles.back());
	}

	for (auto& uni : uniforms)
		glBindAttribLocation(handle, uni.index, uni.name);
	
	glLinkProgram(handle);

	GLint linked;
	glGetProgramiv(handle, GL_LINK_STATUS, &linked);

	if (!linked) {
		char buf[512];
		glGetProgramInfoLog(handle, 512, NULL, buf);
		exit(0);
	}
	// TODO: add debuging logs

	for (auto& shader : shader_handles) {
		glDeleteShader(shader);
	}
}

gal::renderer_opengl::Shader::~Shader() {
	glDeleteProgram(handle);
}

void gal::renderer_opengl::Shader::bind() {
	if (handle == bound_shader_handle) return;
	bound_shader_handle = handle;
	glUseProgram(handle);
}

void gal::renderer_opengl::Shader::unbind() {
	bound_shader_handle = 0;
	glUseProgram(0);
}

void gal::renderer_opengl::Shader::dispatch_compute(uint16_t comp_size_x, uint16_t comp_size_y, uint16_t comp_size_z) {
	if (!is_compute) return;
	// TODO: add error messages

	glDispatchCompute(comp_size_x, comp_size_y, comp_size_z);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

static GLint _get_uniform_location(GLuint handle, std::string name) {
	GLint location;
	const auto& search_cache = uniform_location_cache.find(name);
	if (search_cache != uniform_location_cache.end())
		location = search_cache->second;
	else {
		location = glGetUniformLocation(handle, name.c_str());
		uniform_location_cache.insert({name, location});
	}

	return location;
}

void gal::renderer_opengl::Shader::set(std::string name, glm::mat4 m) {
	glUniformMatrix4fv(_get_uniform_location(handle, name), 1, GL_FALSE, (const GLfloat*) &m);
}

void gal::renderer_opengl::Shader::set(std::string name, glm::vec4 f4) {
	glUniform4f(_get_uniform_location(handle, name), f4.x, f4.y, f4.z, f4.w);
}

void gal::renderer_opengl::Shader::set(std::string name, glm::vec3 f3) {
	glUniform3f(_get_uniform_location(handle, name), f3.x, f3.y, f3.z);
}

void gal::renderer_opengl::Shader::set(std::string name, float f) {
	glUniform1f(_get_uniform_location(handle, name), f);
}

void gal::renderer_opengl::Shader::set(std::string name, uint32_t u) {
	glUniform1ui(_get_uniform_location(handle, name), u);
}

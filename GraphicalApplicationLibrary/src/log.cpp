#include "gal/log.hpp"

#ifdef GAL_LOGGING
#include <string>

namespace gal {
	std::shared_ptr<spdlog::logger> logger = nullptr;
}

void gal::init_logger(std::vector<spdlog::sink_ptr> external_sinks) {
	if (external_sinks.size() > 0) {
		gal::logger = std::make_shared<spdlog::logger>("GAL", external_sinks.begin(), external_sinks.end());
	} else {
		gal::logger = spdlog::basic_logger_mt("GAL", "logs/GAL_Log.txt");
	}

	// [logger name] [log level] [time] - log text
	gal::logger->set_pattern("[%n] [%l] [%T] - %v");
}

void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void *userParam) {
	// ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

	std::string s_source, s_type;

	switch (source) {
		case GL_DEBUG_SOURCE_API:             s_source = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   s_source = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: s_source = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     s_source = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     s_source = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           s_source = "Other"; break;
	}

	switch (type) {
		case GL_DEBUG_TYPE_ERROR:               s_type = "Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: s_type = "Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  s_type = "Undefined Behaviour"; break; 
		case GL_DEBUG_TYPE_PORTABILITY:         s_type = "Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         s_type = "Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              s_type = "Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          s_type = "Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           s_type = "Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               s_type = "Other"; break;
	}
	
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         GAL_LOG_CRITICAL("[OpenGL] [{}] [{}] [{}] - {}", s_source, s_type, id, message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       GAL_LOG_ERROR("[OpenGL] [{}] [{}] [{}] - {}", s_source, s_type, id, message); break;
		case GL_DEBUG_SEVERITY_LOW:          GAL_LOG_WARN("[OpenGL] [{}] [{}] [{}] - {}", s_source, s_type, id, message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: GAL_LOG_INFO("[OpenGL] [{}] [{}] [{}] - {}", s_source, s_type, id, message); break;
	}
}
#endif

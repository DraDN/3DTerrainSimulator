#pragma once

#ifdef GAL_LOGGING
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

namespace gal {
	extern std::shared_ptr<spdlog::logger> logger;

	void init_logger(std::vector<spdlog::sink_ptr> external_sinks = {});
};

void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void *userParam);

#define GAL_LOG_INFO(...) if (gal::logger) gal::logger->info(__VA_ARGS__)
#define GAL_LOG_WARN(...) if (gal::logger) gal::logger->warn(__VA_ARGS__)
#define GAL_LOG_ERROR(...) if (gal::logger) gal::logger->error(__VA_ARGS__)
#define GAL_LOG_CRITICAL(...) if (gal::logger) gal::logger->critical(__VA_ARGS__)

#else
namespace gal {
	inline void init_logger(...) {};
}

#define GAL_LOG_INFO(...) ((void)0)
#define GAL_LOG_WARN(...) ((void)0)
#define GAL_LOG_ERROR(...) ((void)0)
#define GAL_LOG_CRITICAL(...) ((void)0)

#endif
#pragma once

#ifdef NOISELIB_LOGGING
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <vector>

namespace noiselib {
	extern std::shared_ptr<spdlog::logger> logger;

	void init_logger(std::vector<spdlog::sink_ptr> external_sinks = {});
}

#define NLIB_LOG_INFO(...) if (noiselib::logger) noiselib::logger->info(__VA_ARGS__)
#define NLIB_LOG_WARN(...) if (noiselib::logger) noiselib::logger->warn(__VA_ARGS__)
#define NLIB_LOG_ERROR(...) if (noiselib::logger) noiselib::logger->error(__VA_ARGS__)
#define NLIB_LOG_CRITICAL(...) if (noiselib::logger) noiselib::logger->critical(__VA_ARGS__)

#else
namespace noiselib {
	inline void init_logger(...) {};
}

#define NLIB_LOG_INFO(...) ((void)0)
#define NLIB_LOG_WARN(...) ((void)0)
#define NLIB_LOG_ERROR(...) ((void)0)
#define NLIB_LOG_CRITICAL(...) ((void)0)

#endif
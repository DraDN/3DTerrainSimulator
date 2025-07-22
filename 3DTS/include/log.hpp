#pragma once

#ifdef TS_LOGGING
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <memory>
#include <vector>

extern std::shared_ptr<spdlog::logger> logger;

void init_logger();

#define APP_LOG_INFO(...) if (logger) logger->info(__VA_ARGS__)
#define APP_LOG_WARN(...) if (logger) logger->warn(__VA_ARGS__)
#define APP_LOG_ERROR(...) if (logger) logger->error(__VA_ARGS__)
#define APP_LOG_CRITICAL(...) if (logger) logger->critical(__VA_ARGS__)

#else

inline void init_logger(...) {};

#define APP_LOG_INFO(...) ((void)0)
#define APP_LOG_WARN(...) ((void)0)
#define APP_LOG_ERROR(...) ((void)0)
#define APP_LOG_CRITICAL(...) ((void)0)

#endif
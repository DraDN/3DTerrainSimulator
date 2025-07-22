#include "log.hpp"

#ifdef TS_LOGGING
std::shared_ptr<spdlog::logger> logger = nullptr;

#include "noiselib/log.hpp"
#include "gal/log.hpp"

void init_logger() {
	std::vector<spdlog::sink_ptr> app_log_sinks;
	app_log_sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
	app_log_sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/3DTS_Log.txt", true));
	gal::init_logger(app_log_sinks);
	noiselib::init_logger(app_log_sinks);
	
	logger = std::make_shared<spdlog::logger>("3DTS", app_log_sinks.begin(), app_log_sinks.end());
	// [logger name] [log level] [time] - log text
	logger->set_pattern("[%n] [%l] [%T] - %v");
}
#endif
#include "noiselib/log.hpp"

#ifdef NOISELIB_LOGGING
namespace noiselib {
	std::shared_ptr<spdlog::logger> logger = nullptr;
}

void noiselib::init_logger(std::vector<spdlog::sink_ptr> external_sinks) {
	if (external_sinks.size() > 0) {
		noiselib::logger = std::make_shared<spdlog::logger>("NOISELIB", external_sinks.begin(), external_sinks.end());
	} else {
		noiselib::logger = spdlog::basic_logger_mt("GAL", "logs/NOISELIB_Log.txt");
	}

	// [logger name] [log level] [time] - log text
	noiselib::logger->set_pattern("[%n] [%l] [%T] - %v");
}
#endif
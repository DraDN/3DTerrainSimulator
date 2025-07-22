#include "gal/log.hpp"

#ifdef GAL_LOGGING
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
#endif

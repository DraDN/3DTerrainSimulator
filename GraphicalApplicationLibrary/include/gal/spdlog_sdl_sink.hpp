#pragma once

#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <SDL_log.h>
#include <mutex>

template<typename Mutex>
class sdl_sink : public spdlog::sinks::base_sink<Mutex> {
	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override {
			spdlog::memory_buf_t formatted;
			this->formatter_->format(msg, formatted);

			SDL_Log("%s", fmt::to_string(formatted).c_str());
		}
	
		void flush_() override {}
} ;

using sdl_sink_mt = sdl_sink<std::mutex>;
using sdl_sink_st = sdl_sink<spdlog::details::null_mutex>;

#include "lfpch.hpp"
#include "Log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Leaf
{
	void 
	Log::init() noexcept
	{
		s_engineLogger = spdlog::stdout_color_mt("ENGINE");
		s_engineLogger->set_level(spdlog::level::trace);
		s_engineLogger->set_pattern("[%s {%#}] %n: %v%$");

		s_gameLogger = spdlog::stdout_color_mt("GAME");
		s_gameLogger->set_level(spdlog::level::trace);
		s_gameLogger->set_pattern("[%s {%#}] %n: %v%$");
	}
}
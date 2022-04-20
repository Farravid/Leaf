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
#if LF_LOG_EXTENDED
		s_engineLogger->set_pattern("[%s {%#}] %n: %v%$");
#else
		s_engineLogger->set_pattern("%n: %v%$");
#endif
		
		s_gameLogger = spdlog::stdout_color_mt("GAME");
		s_gameLogger->set_level(spdlog::level::trace);
#if LF_LOG_EXTENDED
		s_gameLogger->set_pattern("[%s {%#}] %n: %v%$");
#else
		s_gameLogger->set_pattern("%n: %v%$");
#endif

	}
}
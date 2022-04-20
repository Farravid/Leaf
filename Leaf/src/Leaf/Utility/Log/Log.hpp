/**
TODO
 */
#pragma once

//SPDLOG minimum level of logging
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Leaf {
	class Log
	{
		private:
			inline static std::shared_ptr<spdlog::logger> s_engineLogger	{ nullptr };
			inline static std::shared_ptr<spdlog::logger> s_gameLogger		{ nullptr };

		public:
			/**
			 * @brief Initialize the Leaf Log system
			 */
			static void init() noexcept;
		
			static std::shared_ptr<spdlog::logger> getEngineLogger() 	noexcept { return s_engineLogger; }
			static std::shared_ptr<spdlog::logger> getGameLogger() 	  	noexcept { return s_gameLogger; }
	};
}

//=====================================================
// MACROS
//=====================================================

#if LF_DEBUG

	/*Engine log macros - This should be used by the engine*/
	#define LF_ENGINE_LOG_DEBUG(...)     	SPDLOG_LOGGER_TRACE(Leaf::Log::getEngineLogger(),__VA_ARGS__)
	#define LF_ENGINE_LOG_INFO(...)     	SPDLOG_LOGGER_INFO(Leaf::Log::getEngineLogger(),__VA_ARGS__)
	#define LF_ENGINE_LOG_WARN(...)   		SPDLOG_LOGGER_WARN(Leaf::Log::getEngineLogger(),__VA_ARGS__)
	#define LF_ENGINE_LOG_ERROR(...)    	SPDLOG_LOGGER_ERROR(Leaf::Log::getEngineLogger(),__VA_ARGS__)
	#define LF_ENGINE_LOG_CRITICAL(...)   	SPDLOG_LOGGER_CRITICAL(Leaf::Log::getEngineLogger(),__VA_ARGS__)

	/*Game log macros - This should be used by the game application*/
	#define LF_GAME_LOG_DEBUG(...)     		SPDLOG_LOGGER_TRACE(Leaf::Log::getGameLogger(),__VA_ARGS__)
	#define LF_GAME_LOG_INFO(...)     		SPDLOG_LOGGER_INFO(Leaf::Log::getGameLogger(),__VA_ARGS__)
	#define LF_GAME_LOG_WARN(...)     		SPDLOG_LOGGER_WARN(Leaf::Log::getGameLogger(),__VA_ARGS__)
	#define LF_GAME_LOG_ERROR(...)    		SPDLOG_LOGGER_ERROR(Leaf::Log::getGameLogger(),__VA_ARGS__)
	#define LF_GAME_LOG_CRITICAL(...)   	SPDLOG_LOGGER_CRITICAL(Leaf::Log::getGameLogger(),__VA_ARGS__)

#else

	#define LOG_SIMPLE_OUTPUT
	#define LOG_EXTENDED_OUTPUT

	#define LF_ENGINE_LOG_DEBUG(...)     	
	#define LF_ENGINE_LOG_INFO(...)     	
	#define LF_ENGINE_LOG_WARN(...)     	
	#define LF_ENGINE_LOG_ERROR(...)    	
	#define LF_ENGINE_LOG_CRITICAL(...)   

	#define LF_GAME_LOG_DEBUG(...)     		
	#define LF_GAME_LOG_INFO(...)     		
	#define LF_GAME_LOG_WARN(...)     		
	#define LF_GAME_LOG_ERROR(...)    		
	#define LF_GAME_LOG_CRITICAL(...)

#endif
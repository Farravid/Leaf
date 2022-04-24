//TODO: Description
#pragma once
#include "Log.hpp"

#if _MSC_VER
	#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

//TODO: Check log is init
#if LF_DEBUG
	#define LF_ENGINE_ASSERT(condition, ...)										\
		if(! (condition) )															\
		{																			\
			LF_ENGINE_LOG_CRITICAL("Assertion '{0}' failed", #condition);			\
			LF_ENGINE_LOG_CRITICAL(" in file {0}, line {1}", __FILE__, __LINE__);	\
			LF_ENGINE_LOG_CRITICAL(" function: {0}", __PRETTY_FUNCTION__);			\
			LF_ENGINE_LOG_CRITICAL(" message: {0}", __VA_ARGS__);					\
			std::terminate();														\
		};

	#define LF_GAME_ASSERT(condition, ...)											\
		if(! (condition) )															\
		{																			\
			LF_GAME_LOG_CRITICAL("Assertion '{0}' failed", #condition);				\
			LF_GAME_LOG_CRITICAL(" in file {0}, line {1}", __FILE__, __LINE__);		\
			LF_GAME_LOG_CRITICAL(" function: {0}", __PRETTY_FUNCTION__);			\
			LF_GAME_LOG_CRITICAL(" message: {0}", __VA_ARGS__);						\
			std::terminate();														\
		};
#else
	#define LF_ENGINE_ASSERT(condition, ...)
	#define LF_GAME_ASSERT(condition, ...)
#endif

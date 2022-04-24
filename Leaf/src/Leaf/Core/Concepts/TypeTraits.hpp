#pragma once

#include "lfpch.hpp"
#include "Metaprogramming/Metaprogramming.hpp"

namespace Leaf::core
{
	namespace
	{
		template<typename AlwaysVoid, template <typename...> typename Detectable, typename... Args>
		struct detector
			: mp::type_return<std::false_type> {};

		template<template <typename...> typename Detectable, typename... Args>
		struct detector<std::void_t<Detectable<Args...>>, Detectable, Args...>
			: mp::type_return<std::true_type> {};
	}

	template<template <typename...> typename Detectable, typename... Args>
	using is_detected = typename detector<void, Detectable, Args...>::type;

	template<template <typename...> typename Detectable, typename... Args>
	constexpr auto is_detected_v = is_detected<Detectable, Args...>::value;
}

#define GENERATE_CLASS_HAS_FUNCTION( FN )												\
	template<typename Class>															\
	using FN##_detector = decltype( std::declval<Class&>().FN() );						\
	template<typename Class>															\
	constexpr bool has_func_##FN##_v = Leaf::core::is_detected_v<FN##_detector, Class>;	\

#define GEN_TRAIT_CLASS_HAS_MEMBER( MEMBER )														\
	template<typename Class>																		\
	using MEMBER##_detector = decltype( std::declval<Class&>().MEMBER );							\
	template<typename Class, typename MemberType>													\
	constexpr bool has_member_##MEMBER##_v = Leaf::core::is_detected_v<MEMBER##_detector, Class> &	\
											 std::is_same_v<MEMBER##_detector<Class>, MemberType>;	\

#define GEN_TRAIT_CLASS_HAS_MEMBER_CUSTOM( MEMBER )													\
	template<typename Class>																		\
	using MEMBER##_detector = decltype(Class::MEMBER);												\
	template<typename Class, template <typename...> typename TypeTrait>								\
	constexpr bool has_member_##MEMBER##_v = Leaf::core::is_detected_v<MEMBER##_detector, Class> &	\
											 TypeTrait<MEMBER##_detector<Class>>::value;			\

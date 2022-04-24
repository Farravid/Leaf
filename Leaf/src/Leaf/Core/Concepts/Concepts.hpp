#pragma once

#include "lfpch.hpp"
#include "TypeTraits.hpp"

namespace Leaf::core
{
	template<typename T>
	concept copyable	= std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>;

	template<typename T>
	concept movable		= std::is_move_assignable_v<T> && std::is_move_constructible_v<T>;

	template<typename T, typename... Types>
	concept one_of = (std::is_same_v<T, Types> || ...);
}

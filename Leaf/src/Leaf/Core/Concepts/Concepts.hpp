#pragma once

#include "lfpch.hpp"

namespace Leaf
{
	template<typename T>
	concept Copyable	= std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>;

	template<typename T>
	concept Movable		= std::is_move_assignable_v<T> && std::is_move_constructible_v<T>;
}

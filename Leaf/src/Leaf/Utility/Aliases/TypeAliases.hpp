/**
 * @file TypeAliases.hpp
 * @author David (davidmg103@gmail.com)
 * @brief Type aliases for using in the engine
 * @version 0.1
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once 

#include "lfpch.hpp"

namespace Leaf
{
	//=========================================================================
	//STANDARD ALIASES
	//=========================================================================

	template<typename T>
	using UniqPtr = std::unique_ptr<T>;
	template<typename T>
	using RefWrapper = std::reference_wrapper<T>;
	template<typename T, typename U>
	using Map = std::map<T, U>;
	template<typename T, typename U>
	using UnorderedMap = std::unordered_map<T, U>;
	template<typename... T>
	using Tuple = std::tuple<T...>;
}


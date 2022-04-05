/**
 * @file TypeList.hpp
 * @author OcachoGames-David ( ocachogames@gmai.com)
 * @brief Common metaprogramming functions.
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

namespace Leaf::mp
{
    /**
     * @brief Return a compile time variable in value
     * 
     * @tparam Type Type of the variable
     * @tparam Value Value of the variable
     */
	template<typename Type, Type Value>
	struct constant { static constexpr Type value {Value}; };

	/**
	 * @brief Return the given type in type
	 * 
	 * @tparam T Type to return
	 */
	template<typename T>
	struct type_return { using type = T; };
}

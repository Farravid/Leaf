/**
 * @file Conditional.hpp
 * @author OcachoGames-David ( ocachogames@gmai.com)
 * @brief Metaprogramming functions for conditionals statements.
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "Metaprogramming/Common.hpp"

namespace Leaf::mp
{
    /**
     * @brief Given a condition, return the TypeTrue if the condition is true
     * or return TypeFalse if the condition is false
     * 
     * @tparam Condition Condition to check
     * @tparam TypeTrue Type in case true
     * @tparam TypeFalse Type in case false
     */
	template<bool Condition, typename TypeTrue, typename TypeFalse>
	struct if_condition_type {};
	template<typename TypeTrue, typename TypeFalse>
	struct if_condition_type<true,TypeTrue,TypeFalse>
		: type_return<TypeTrue>
	{};
	template<typename TypeTrue, typename TypeFalse>
	struct if_condition_type<false,TypeTrue,TypeFalse>
		: type_return<TypeFalse>
	{};
	template<bool Condition, typename TypeTrue, typename TypeFalse>
	using if_condition_type_t = typename if_condition_type<Condition,TypeTrue,TypeFalse>::type;
}
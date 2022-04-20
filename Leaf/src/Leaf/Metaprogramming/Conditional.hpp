/**
 * @file Conditional.hpp
 * @author David (davidmg103@gmail.com)
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
	
	/**
	* @brief Select the better unsigned storage type(uint8_t, uint16_t)... given a static non - variable size.
	*
	* @tparam Size Size to calculate the type
	*/
	template<size_t Size>
	struct select_ustorage_type
	{
		using type = if_condition_type_t<(Size <= 8), uint8_t,
			if_condition_type_t<(Size <= 16), uint16_t,
			if_condition_type_t<(Size <= 32), uint32_t, uint64_t>>>;
	};
	template<size_t Size>
	using select_ustorage_type_t = typename select_ustorage_type<Size>::type;

	/**
	* @brief Select the better signed storage type(int8_t, int16_t)... given a static non - variable size.
	*
	* @tparam Size Size to calculate the type
	*/
	template<size_t Size>
	struct select_storage_type
	{
		using type = if_condition_type_t<(Size <= 8), int8_t,
			if_condition_type_t<(Size <= 16), int16_t,
			if_condition_type_t<(Size <= 32), int32_t, int64_t>>>;
	};
	template<size_t Size>
	using select_storage_type_t = typename select_ustorage_type<Size>::type;
}
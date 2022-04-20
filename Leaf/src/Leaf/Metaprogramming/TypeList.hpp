/**
 * @file TypeList.hpp
 * @author David (davidmg103@gmail.com)
 * @brief Metaprogramming functions for List of Types.
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "Metaprogramming/Common.hpp"
#include "Metaprogramming/Conditional.hpp"

namespace Leaf::mp
{

	/**
	 * @brief Get the position of a type given a Type and a List of types
	 * Used as the ID of the type
	 * 
	 * @tparam Type Type to look up for
	 * @tparam Types List of types
	 */
	template<typename Type, typename... Types>
	struct pos_type{ static_assert(sizeof...(Types),"List of types is empty or the Type is not contained in the list"); };
	template<typename Type, typename... Types>
	struct pos_type<Type,Type,Types...> 
		: constant<std::size_t,0> {};
	template<typename Type, typename OtherType, typename... Types>
	struct pos_type<Type,OtherType,Types...>
		: constant<std::size_t, (1 + pos_type<Type,Types...>::value)> {};
	template<typename Type, typename... Types>
	constexpr std::size_t pos_type_v = pos_type<Type,Types...>::value;

	//=========================================================================
	// TypeList: struct to handle list of types
	//=========================================================================
	template<typename... Types>
	struct TypeList 
	{
		/**
		 * @brief Get the size of the TypeList
		 * 
		 * @return consteval The size of the typelist
		 */
		consteval static std::size_t 
		size() { return sizeof...(Types); }
	
		/**
		 * @brief Whether the TypeList contains the given Type or not
		 * at compile time
		 * 
		 * @tparam Type Time to look up for
		 * @return consteval uint8_t 1 if the type is contained, otherwise 0 
		 */
		template<typename Type>
		consteval static uint8_t 
		contains() { return 0 || (std::is_same_v<Type,Types> || ...); }

		/**
		 * @brief Return the position of the Type on the list.
		 * Used as IDs.
		 * 
		 * @tparam Type Type to look up for 
		 */
		template<typename Type>
		consteval static std::size_t 
		position() { return pos_type_v<Type,Types...>; }
	};


	//=========================================================================
	// TypeListTraits: TypeList with additional features as IDs and Masks
	//=========================================================================

	template<typename TypeList>
	struct TypeListTraits
	{
		//Type of the mask to use (uint8_t, uint16_t...) depending on the size of the list;
		using mask_type = select_ustorage_type_t<TypeList::size()>;

		/**
		 * @brief Get the size of the TypeListTraits
		 * 
		 * @return consteval The size of the typelist
		 */
		consteval static std::size_t 
		size() { return TypeList::size(); } 
		
		/**
		 * @brief ID of the type on the list
		 * 
		 * @tparam Type Type to look up for
		 */
		template<typename Type>
		consteval static std::size_t 
		id() { return TypeList::template position<Type>(); }

		/**
		 * @brief Mask of the type on the list
		 * 
		 * @tparam Type Type to look up for
		 */
		template<typename Type>
		consteval static std::size_t 
		getSingleListTypeMask() { return 1 << id<Type>(); }

		/**
		 * @brief Masks of the types on the list
		 * 
		 * @tparam Types Types to look up for
		 */
		template<typename... Types>
		consteval static std::size_t 
		getListTypesMask()
		{
			return ( 0 | ... | (getSingleListTypeMask<Types>()) );
		}
	};
}


/**
 * @file Replacement.hpp
 * @author David (davidmg103@gmail.com)
 * @brief Metaprogramming functions for types replacement.
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "Common.hpp"
#include "TypeList.hpp"

namespace Leaf::mp
{
	/**
	 * @brief Change the type of a container by another one
	 * Example: container_replace_t<std::list,std::vector<int,float>> == std::list<int,float>
	 * 
	 * @tparam NewContainer 
	 * @tparam OldContainer 
	 */
	template<template <typename...> typename NewContainer,
			 typename OldContainer>
	struct container_replace {};
	template<template <typename...> typename NewContainer,
			 template <typename...> typename OldContainer,
			 typename... Types>
	struct container_replace<NewContainer,OldContainer<Types...>>
		: type_return <NewContainer<Types...>>
	{};
	template<template <typename...> typename NewContainer,
			 typename OldContainer>
	using container_replace_t = typename container_replace<NewContainer,OldContainer>::type;

	/**
	 * @brief Given a typelist, for each element of the list, change
	 * their types.
	 * Example: 
	 * 	using CMPs = TypeList<PhyCmp,InputCmp,RenderCmp>
	 * 	for_all_elements_list_replace_t<std::vector,CMPs> == TypeList<std::vector<PhyCmp>,std::vector<InputCmp>....>
	 * 
	 * @tparam Class_t New inner types
	 * @tparam List List to change the inner types
	 */
	template<template <typename... Ts> typename Class_t,
				typename List>
	struct for_all_elements_list_replace{};
	template<template <typename... Ts> typename Class_t,
				typename... Types>
	struct for_all_elements_list_replace<Class_t, TypeList<Types...>>
		: type_return <TypeList<Class_t<Types>...>>
	{};
	template<template <typename... Ts> typename Class_t,
				typename... Types>
	using for_all_elements_list_replace_t = typename for_all_elements_list_replace<Class_t,Types...>::type;

	/**
	 * @brief Given a new container and a typelist, change the type "TypeList"
	 * by the given New Container
	 * Example:
	 * 		using CMPs = TypeList<PhyCmp,InputCmp,RenderCmp>
	 * 		replace_list_t<std::tuple,CMPs> == std::tuple<PhyCmp,InputCmp,RenderCmp>
	 * 
	 * @tparam NewContainer New container
	 * @tparam List Type list
	 */
	template<template <typename...> typename NewContainer,
			 typename List>
	struct replace_list{};
	template<template <typename...> typename NewContainer,
			 typename... Types>
	struct replace_list<NewContainer,TypeList<Types...>>
		: type_return <NewContainer<Types...>>
	{};
	template<template <typename...> typename NewContainer,
			 typename List>
	using replace_list_t = typename replace_list<NewContainer,List>::type;

	/**
	 * @brief Given a new container and a typelist, change the type "TypeList"
	 * by the given New Container.
	 * Also change the inner type of the types by the InnerContainer_t
	 * 
	 * Example:
	 * 		using CMPs = TypeList<PhyCmp,InputCmp,RenderCmp>
	 * 		replace_all_list_t<std::tuple,std::vector,CMPs> == std::tuple<std::vector<PhyCmp>,std::vector<InputCmp>...>
	 * 
	 * @tparam Container_t New container
	 * @tparam InnerContainer_t New inner container
	 * @tparam TypeList Type list
	 */
	template <template <typename...> typename Container_t,
			template <typename...> typename InnerContainer_t,
			typename TypeList>
	struct replace_all_list {};
	template <template <typename...> typename Container_t,
			template <typename...> typename InnerContainer_t,
			typename... Types>
	struct replace_all_list<Container_t,InnerContainer_t, TypeList<Types...>>
		: type_return <Container_t<InnerContainer_t<Types>...>>
	{};
	template <template <typename...> typename Container_t,
			template <typename...> typename InnerContainer_t,
			typename TypeList>
	using replace_all_list_t = typename replace_all_list<Container_t, InnerContainer_t, TypeList>::type;

	/**
	 * @brief Add new types to a type list
	 * 
	 * @tparam NewType New type to the list
	 * @tparam TypeList Type list to add the new type
	 */
	template<typename TypeList, typename... NewTypes>
	struct add_types_to_list {};
	template<typename... Types, typename... NewTypes>
	struct add_types_to_list<TypeList<Types...>, NewTypes...>
		: type_return <TypeList<Types...,NewTypes...>>
	{};
	template<typename TypeList, typename... NewTypes>
	using add_types_to_list_t = typename add_types_to_list<TypeList,NewTypes...>::type;

	/**
	 * @brief Combine two type lists
	 * 
	 * @tparam TypeList1 First type list
	 * @tparam TypeList2 Second type list
	 */
	template<typename TypeList1, typename TypeList2>
	struct combine_type_list {};
	template<typename... Types1, typename... Types2>
	struct combine_type_list<TypeList<Types1...>, TypeList<Types2...>>
		: type_return <TypeList<Types1...,Types2...>>
	{};
	template<typename TypeList1, typename TypeList2>
	using combine_type_list_t = typename combine_type_list<TypeList1,TypeList2>::type;

}
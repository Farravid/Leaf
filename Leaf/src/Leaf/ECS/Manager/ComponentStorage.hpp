/**
 * @file ComponentStorage.hpp
 * @author OcachoGames-David (ocachogames@gmail.com)
 * @brief Class where all the components from the entities are storaged
 * 
 * Besides, this class holds all the implementation for managing components.
 * 
 * @version 0.1
 * @date 2021-12-07 
 *  
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "lfpch.hpp"

#include "Metaprogramming/Metaprogramming.hpp"
#include "DataStructures/SlotMap.hpp"
#include "Macros/ClassesMacros.hpp"

namespace Leaf::ecs
{
	template <typename CmpsList, typename SingletonCmpsList, std::size_t Capacity>
	struct ComponentStorage
	{
		//=========================================================================
		// Type list info
		//=========================================================================
		using cmpsinfo   		= typename mp::TypeListTraits<CmpsList>;

		//=========================================================================
		// Component storage type
		//=========================================================================
		template <typename T>
		using StorageType 			= typename DataStructures::SlotMap<T, typename cmpsinfo::mask_type, Capacity>;
		using Storage_t 			= mp::replace_all_list_t<std::tuple,StorageType,CmpsList>;
		using SingletonStorage_t 	= mp::replace_list_t<std::tuple, SingletonCmpsList>; 

		private:
			//Components
			Storage_t 			components_{}; 
			//Singleton components
			SingletonStorage_t	singletonComponents_{};

		private:
			/**
			 * @brief getSingletonComponent implementation. See description below
			 */
			template<typename CMP>
			static constexpr auto&
			getComponent_impl(auto* self, auto p_slotkey) noexcept;

			/**
			 * @brief getSingletonComponent implementation. See description below
			 */
			template<typename SingletonCMP>
			static constexpr auto&
			getSingletonComponent_impl(auto* self) noexcept;

			/**
			 * @brief getComponents implementation. See description below
			 */
			template<typename CMP>
			static constexpr auto&
			getComponents_impl(auto* self) noexcept;

		public:
            /**
             * @brief Create a Component Storage 
             */
			explicit constexpr ComponentStorage() = default;

			//Making this class non copiable
			NON_COPIABLE_CLASS(ComponentStorage)
			
			/**
			 * @brief Given a CMP type and the Constructor params to build it.
			 * Add a component to the component storage
			 * 
			 * @tparam CMP Type of the component
			 * @tparam ConstructorParams Types of the cmp's constructor params 
			 * @param p_params Constructor params
			 * @return auto A tuple with the slotmap key and the pointer to the component
			 */
			template<typename CMP, typename... ConstructorParams>
			constexpr auto 
			addComponent(ConstructorParams&&... p_params) noexcept;

			/**
			 * @brief Given the type of the CMP and the slotmap key
			 * remove the component associated to that key
			 * 
			 * @tparam CMP Type of the component
			 * @param p_slotKey Slotmap key
			 */
			template<typename CMP>
			constexpr void
			removeComponent(auto p_slotKey) noexcept;

			/**
			 * @brief Get an specific component from the storage
			 * 
			 * @tparam CMP Type of the component
			 * @param p_slotKey Key of the storage (slotmap)
			 * @return constexpr CMP& Component as reference
			 */
			template<typename CMP>
			constexpr const CMP&
			getComponent(auto p_slotKey) const noexcept { return getComponent_impl<CMP>(this, p_slotKey); }
			template<typename CMP>
			constexpr CMP&
			getComponent(auto p_slotKey) noexcept { return getComponent_impl<CMP>(this, p_slotKey); }

			/**
			 * @brief Get an specific singleton component from the singleton storage
			 * 
			 * @tparam SingletonCMP Type of the component
			 * @return constexpr SingletonCMP& Component as reference
			 */
			template<typename SingletonCMP>
			constexpr const SingletonCMP&
			getSingletonComponent() const noexcept { return getSingletonComponent_impl<SingletonCMP>(this); }
			template<typename SingletonCMP>
			constexpr SingletonCMP&
			getSingletonComponent() noexcept { return getSingletonComponent_impl<SingletonCMP>(this); }

			/**
			 * @brief Get all the given type components 
			 * 
			 * @tparam CMP Component type
			 * @return constexpr const StorageType<CMP>& Slotmap with the CMP type components 
			 */
			template<typename CMP>
			constexpr const StorageType<CMP>&
			getComponents() const noexcept { return getComponents_impl<CMP>(this); }
			template<typename CMP>
			constexpr StorageType<CMP>&
			getComponents() noexcept { return getComponents_impl<CMP>(this); }

			/**
			 * @brief Clean all the component storages
			 */
			constexpr void
			clearStorage() noexcept;
	};
}

#include "ComponentStorage.tpp"
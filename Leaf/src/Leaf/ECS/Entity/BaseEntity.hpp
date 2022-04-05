/**
 * @file BaseEntity.hpp
 * @author OcachoGames-David (ocachogames@gmail.com)
 * @brief Base entity for our ECS.
 * 
 * The user of the engine can use this class as the entity or can derive from this class
 * in order to implement different funcionalities.
 * 
 * This class holds a basic implementation for an entity. The ID of the entity and the map of components
 * 
 * @version 0.1
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "DataStructures/SlotMap.hpp"
#include "Metaprogramming/Replacement.hpp"

#define NON_VALID_ENTITY_ID 0

namespace Leaf::ecs
{
    template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpsList, size_t Capacity>
	class BaseEntity
	{
		public:
			template<typename CL, typename SCL, typename TL, size_t C>
			friend class EntityManager;

		private:
			//=========================================================================
			// Components key storage
			//=========================================================================
			template <typename T>
			using key_type_t 		= typename DataStructures::SlotMap<T,TypeMaskCmps,Capacity>::key_type;
			
			using CmpKeysStorage_t 	= mp::replace_all_list_t<std::tuple,key_type_t,CmpsList>;

			//=========================================================================
			// Variables
			//=========================================================================
			//Static counter for entity ID
			inline static size_t nextID { 0 };
			//The ID of the entity itself
            std::size_t id_ { ++nextID };

			//Entity's components
			TypeMaskCmps components_{0};
			//Entity's tags
			TypeMaskTags tags_{0};
			//Entity;s components slotmap keys
			CmpKeysStorage_t componentKeys_{};

		public:
			/**
			 * @brief Create an BaseEntity 
			 */
			explicit constexpr BaseEntity() = default;

			//Equality operator
			bool
			operator==(const BaseEntity&) const noexcept; 
			
			/**
			 * @brief Get the ID of this entity
			 * 
			 * @return std::size_t Entity's ID
			 */
			constexpr std::size_t 
			getID() const noexcept;
			
		private:

			/**
			 * @brief Given the slot map key of the component and the mask of that cmp,
			 * add the component to the entity
			 * 
			 * @param slotKey Slot map key
			 * @param p_cmpMask Cmp's mask
			 */
			template<TypeMaskCmps CMPMask>
			constexpr void 
			addComponent(auto slotKey) noexcept;

			/**
			 * @brief Given the mask of that cmp, remove the component from the entity
			 * 
			 * @param p_cmpMask Cmp's mask
			 */
			template<TypeMaskCmps CMPMask>
			constexpr void 
			removeComponent() noexcept;

			/**
			 * @brief Get the slotmap component key of the cmp
			 * 
			 * @tparam CMP Type of the component
			 * @return constexpr auto Key of the slotmap to look up for the cmp
			 */
			template<typename CMP>
			constexpr auto
			getComponent() const noexcept;

			/**
			 * @brief Get the all the slotmap component key of the cmps in a tuple
			 * 
			 * @return constexpr auto& Tuple with the slotmap component key for each cmp
			 */
			constexpr auto&
			getComponentKeys() const noexcept;

			/**
			 * @brief Chekc if the entity contains the given components mask or not
			 * 
			 * @param p_cmpsMask Mask of the components
			 * @return constexpr uint8_t Whether the entity contains the given components mask or not
			 */
			template<TypeMaskCmps CMPsMask>
			constexpr uint8_t
			containsComponents() const noexcept;

			/**
			 * @brief Given the mask of some tags. 
			 * Add the tags tp the entity 
			 * 
			 * @param p_tagsMask Mask of the tags
			 */
			template<TypeMaskTags TAGsMask>
			constexpr void
			addTags() noexcept;

			/**
			 * @brief Given the mask of some tags. 
			 * Remove the matching tags from the entity 
			 * 
			 * @param p_tagsMask Mask of the tags
			 */
			template<TypeMaskTags TAGsMask>
			constexpr void
			removeTags() noexcept;

			/**
			 * @brief Chekc if the entity contains the given tags mask or not
			 * 
			 * @param p_cmpsMask Mask of the tags
			 * @return constexpr uint8_t Whether the entity contains the given tags mask or not
			 */
			template<TypeMaskTags TAGsMask>
			constexpr bool
			containsTags() const noexcept;

			/**
			 * @brief Chekc if the entity contains the any of the tags in the given mask
			 * 
			 * @param p_cmpsMask Mask of the tags
			 * @return constexpr uint8_t Whether the entity contains the given tags mask or not
			 */
			template<TypeMaskTags TAGsMask>
			constexpr bool
			containsAnyTag() const noexcept;
	};
}

#include "BaseEntity.tpp"
/**
 * @file EntityManager.hpp
 * @author OcachoGames-David (ocachogames@gmail.com)
 * @brief Manager for Ocacho entities.
 *
 * This class holds all the implementation and logic behving creating, removing and managing entities.
 * Also provides a set of funtionalities to manage components and tags.
 * 
 * @version 0.1
 * @date 2021-12-07 
 *  
 * @copyright Copyright (c) 2021
 */

//TODO: Const correctness cuando fran explique lo del const_cast

#pragma once

#include "ComponentStorage.hpp"
#include "ECS/Entity/BaseEntity.hpp"

namespace Leaf::ecs
{
	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity = 1000>
	struct EntityManager
	{
		//=========================================================================
		// Flags matching for the update
		//=========================================================================
		enum class TagsFlagMatching : uint8_t
		{
			AllTagsRequired,
			OneTagRequired
		};

		//=========================================================================
		// Component storage specific instantiation
		//=========================================================================
		using ComponentStorage_t = ComponentStorage<CmpsList,SingletonCmpsList,Capacity>;

		//=========================================================================
		// Type list info
		//=========================================================================
		using tagsinfo           = mp::TypeListTraits<TagsList>;
		using cmpsinfo           = typename ComponentStorage_t::cmpsinfo;

		//=========================================================================
		// Entities types managment
		//=========================================================================
		using Entity_t = BaseEntity<typename cmpsinfo::mask_type,
								  	typename tagsinfo::mask_type,
									CmpsList, Capacity>;

		using EntityStorage_t 	= std::vector<Entity_t>;

		private:
			//Components storage
			ComponentStorage_t componentStorage_{};
			//Entities
			EntityStorage_t entities_{};
		
		private:

			//=========================================================================
			// For all loop implementations private
			//=========================================================================

			/**
			 * @brief Implementation of "forAllMatching". See description on that member function
			 * 
			 * @tparam Cmps Types of components
			 * @tparam Tags Types of tags
			 * @param p_function Function to call if the entity matchs
			 */
			template<TagsFlagMatching TagsFlag, typename... Cmps, typename... Tags>
			constexpr void
			forAllMatching_impl(auto&& p_function, mp::TypeList<Cmps...>,
												   mp::TypeList<Tags...>) noexcept;
			
			/**
			 * @brief Implementation of "forAllMatchingPairs". See description on that member function
			 * 
			 * @tparam TagsFlagLeft Tags matching option for the left entity 
			 * @tparam TagsFlagRight Tags matching option for the right entity
			 * @tparam CmpsLeft Components' signature for the left entity
			 * @tparam TagsLeft Tags' signature for the left entity
			 * @tparam CmpsRight Components' signature for the right entity
			 * @tparam TagsRight Tags' signature for the right entity
			 * @param p_function 
			 */
			template<TagsFlagMatching TagsFlagLeft, TagsFlagMatching TagsFlagRight,
			 		 typename... CmpsLeft, typename... TagsLeft,
					 typename... CmpsRight, typename... TagsRight>
			constexpr void
			forAllMatchingPairs_impl(auto&& p_function, mp::TypeList<CmpsLeft...>,
												   		mp::TypeList<TagsLeft...>,
														mp::TypeList<CmpsRight...>,
														mp::TypeList<TagsRight...>) noexcept;

			/**
			 * @brief Check if a entity is matching the whole signature of a system.
			 * Tags and components included.
			 * 
			 * @tparam TagsFlag Tags matching option for the entity
			 * @tparam Cmps Components to match
			 * @tparam Tags Tags to match
			 * @param entity Entity to check the matching
			 * @return true The entity is matching the whole signature 
			 * @return false The entity isn't matching the whole signature
			 */
			template<TagsFlagMatching TagsFlag, typename... Cmps, typename... Tags>
			constexpr bool
			isEntityMatchingTheSignature(Entity_t& entity, mp::TypeList<Cmps...>,
										   				   mp::TypeList<Tags...>) const noexcept;

			//=========================================================================
			// Entities managment methods implementations
			//=========================================================================

			/**
			 * @brief getEntityByID implementation. See description below
			 */
			static constexpr auto*
			getEntityByID_impl(auto* self, const std::size_t p_id) noexcept; 

			/**
			 * @brief getEntityByName implementation. See description below
			 */
			static constexpr auto*
			getEntityByName_impl(auto* self, const std::string_view& p_name) noexcept; 

			/**
			 * @brief Remove one component from an entity
			 * 
			 * @tparam CMP Type of the cmp to remove
			 * @param p_entity Entity to remove the components from
			 */
			template<typename CMP>
			constexpr void
			removeComponentFromEntity(Entity_t& p_entity) noexcept;

			/**
			 * @brief Remove the components from an entity
			 * 
			 * @tparam CMPs Types of the CMPList
			 * @param p_entity Entity to remove
			 */
			template<typename... CMPs>
			constexpr void
			removeComponentsFromEntity(mp::TypeList<CMPs...>, Entity_t& p_entity) noexcept;

			/**
			 * @brief Remove all the entities marked for destroy
			 */
			constexpr void
			removeDeadEntities() noexcept;

			/**
			 * @brief Copy the CMP component from p_entity to p_newEntity
			 * 
			 * @tparam CMP Type of the component to copy
			 * @param p_entity From entity
			 * @param p_newEntity To entity
			 */
			template<typename CMP>
			constexpr void
			copyEntityComponent(Entity_t& p_entity, Entity_t& p_newEntity) noexcept;

			/**
			 * @brief Copy the tags and components from one entity to another one.
			 * 
			 * @tparam CMPs CMPs list
			 * @tparam TAGs Tags list
			 * @param p_entity Entity to get the components from
			 * @param p_newEntity Entity to copy the components to
			 */
			template<typename... CMPs, typename... TAGs>
			constexpr void
			copyEntityComponentsAndTags(mp::TypeList<CMPs...>, mp::TypeList<TAGs...>,
										Entity_t& p_entity, Entity_t& p_newEntity) noexcept;

			/**
			 * @brief getComponent implementation. See description below
			 */
			template<typename CMP>
			static constexpr decltype(auto)
			getComponent_impl(auto* self, const Entity_t& p_entity) noexcept;

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
			static constexpr decltype(auto)
			getComponents_impl(auto* self) noexcept;

			//=========================================================================
			// SFINAE technique to call reset of cmps if it is present
			//=========================================================================

			/**
			 * @brief If the function void reset() is present in the components, it is called.
			 * Otherwise just continue. Called when a component is gonna be removed
			 * 
			 * @tparam T Type of the cmp
			 * @param cmp Cmp itself
			 * @return decltype(cmp->reset()) void probably 
			 */
			template <class T>
			auto callResetIfPresent(T* cmp) -> decltype(cmp->reset()) { cmp->reset(); }
			void callResetIfPresent(...) { }
	
		public:
			/**
			 * @brief Create an EntityManager 
			 */
			explicit EntityManager(const size_t p_numEntities = 100);

			//Making this class non copiable
			NON_COPIABLE_CLASS(EntityManager)

			//=========================================================================
			// For all loop implementations public
			//=========================================================================

			/**
			 * @brief For all entities matching the system signature of SigCmps and SigTags
			 * call the update system of the system passing the right components of the system's signature
			 * 
			 * @tparam SigCmps Components signature of the components as a type list
			 * @tparam SigTags Components signature of the tags as a type list
			 * @param p_function Function to call if the entity matchs
			 */
			template<typename SigCmps, typename SigTags, TagsFlagMatching TagsFlag>
			constexpr void
			forAllMatching(auto&& p_function) noexcept;

			/**
			 * @brief For all entities matching the left signature, check all entities matching the right
			 * signature and call the callback funciton with the left and right entities and the right components for
			 * each entity.
			 * 
			 * @tparam SigCmpsLeft Components signature of the left entity as a type list
			 * @tparam SigTagsLeft Tags signature of the left entity as a type list
			 * @tparam TagsFlagLeft Tags matching option for the left entity
			 * @tparam SigCmpsRight Components signature of the right entity as a type list
			 * @tparam SigTagsRight Tags signature of the right entity as a type list
			 * @tparam TagsFlagRight Tags matching option for the right entity
			 * @param p_function Function to call if the entities match
			 */
			template<typename SigCmpsLeft,  typename SigTagsLeft,  TagsFlagMatching TagsFlagLeft,
					 typename SigCmpsRight, typename SigTagsRight, TagsFlagMatching TagsFlagRight>
			constexpr void
			forAllMatchingPairs(auto&& p_function) noexcept;

			//=========================================================================
			// Entities managment methods
			//=========================================================================

			/**
			 * @brief Check if should destroy an entity, in case the entity is marked for destroy
			 */
			constexpr void
			update() noexcept;

			/**
			 * @brief Create a Entity object
			 * 
			 * @return constexpr Entity& Entity as reference to modify it.
			 */
			constexpr Entity_t&
			createEntity() noexcept;

			/**
			 * @brief Set ID to entity
			 *
			 * @param p_entity
			 * @param p_id
			 */
			constexpr void
			setEntityID(Entity_t& p_entity, size_t p_id) noexcept;

			/**
			 * @brief Get the Entities object
			 * 
			 * @return constexpr const EntityStorage_t& As const reference to not modify them
			 */
			constexpr const EntityStorage_t&
			getEntities() const noexcept { return entities_; }
			constexpr EntityStorage_t&
			getEntities() noexcept { return entities_; }

			/**
			 * @brief Get the Entity by the entity's id
			 * 
			 * @param p_id Entity's id 
			 * @return constexpr const Entity_t* As const reference 
			 */
			constexpr const Entity_t*
			getEntityByID(const std::size_t p_id) const noexcept { return getEntityByID_impl(this, p_id); } 
			constexpr Entity_t*
			getEntityByID(const std::size_t p_id) noexcept { return getEntityByID_impl(this, p_id); } 
			
			/**
			 * @brief Get the Entity by the entity's name
			 * 
			 * @param p_name Entity's name 
			 * @return constexpr const Entity_t* As const reference 
			 */
			constexpr const Entity_t*
			getEntityByName(const std::string_view& p_name) const noexcept { return getEntityByName_impl(this, p_name); }
			constexpr Entity_t*
			getEntityByName(const std::string_view& p_name) noexcept { return getEntityByName_impl(this, p_name); }

			/**
			 * @brief Given an entity, a component type and the params for building the component,
			 * add the component to the given entity
			 * 
			 * @tparam CMP Type of the component
			 * @tparam ConstructorParams Types of the constructor params for the component
			 * @param p_entity Entity to add the cmp
			 * @param p_params Cmp's constructor params
			 * @return constexpr CMP& Cmp as reference to modify it
			 */
			template<typename CMP, typename... ConstructorParams>
			constexpr CMP&
			addComponent(Entity_t& p_entity, ConstructorParams&&... p_params) noexcept;

			/**
			 * @brief If the entity has the a component of CMP type.
			 * Remove it.
			 * 
			 * @tparam CMP Type of the component to remove 
			 * @param p_entity Entity as reference
			 */
			template<typename CMP>
			constexpr void
			removeComponent(Entity_t& p_entity) noexcept;

			/**
			 * @brief Given an entity, get the CMP component of that entity
			 * 
			 * @tparam CMP Type of the cmp to get
			 * @param p_entity Entity to check
			 * @return constexpr CMP& Cmp as reference to modify it
			 */
			template<typename CMP>
			constexpr const CMP&
			getComponent(const Entity_t& p_entity) const noexcept { return getComponent_impl<CMP>(this, p_entity); }
			template<typename CMP>
			constexpr CMP&
			getComponent(Entity_t& p_entity) noexcept { return getComponent_impl<CMP>(this, p_entity); }

			/**
			 * @brief Get the SingletonCMP component
			 * 
			 * @tparam SingletonCMP Type of the cmp to get
			 * @return constexpr SingletonCMP& Cmp as reference to modify it
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
			 * @return auto& Slotmap with the CMP type components 
			 */
			template<typename CMP>
			constexpr const auto&
			getComponents() const noexcept { return getComponents_impl<CMP>(this); }
			template<typename CMP>
			constexpr auto&
			getComponents() noexcept { return getComponents_impl<CMP>(this); }

			/**
			 * @brief Check if the given entity contains the CMPs given by parameter pack
			 * Const version
			 * 
			 * @tparam CMPs Types of the components
			 * @param p_entity Entity to check 
			 * @return constexpr uint8_t Whether the given entity contains the CMPs given by parameter pack
			 */
			template<typename... CMPs>
			constexpr bool
			containsComponents(const Entity_t& p_entity) const noexcept;

			/**
			 * @brief Given a entity and a list of tags,
			 * add those tags to the given entity.
			 * 
			 * @tparam Tags Tags to add
			 * @param p_entity Target to add the tags
			 */
			template<typename... Tags>
			constexpr void
			addTags(Entity_t& p_entity) const noexcept;

			/**
			 * @brief Given a entity and a list of tags,
			 * remove those tags from the given entity.
			 * 
			 * @tparam Tags Tags to remove
			 * @param p_entity Target to remove tags from
			 */
			template<typename... Tags>
			constexpr void
			removeTags(Entity_t& p_entity) const noexcept;

			/**
			 * @brief Check if the given entity contains the Tags given by parameter pack
			 * 
			 * @tparam Tags Types of the components
			 * @param p_entity Entity to check 
			 * @return constexpr uint8_t Whether the given entity contains the Tags given by parameter pack
			 */
			template<typename... Tags>
			constexpr uint8_t
			containsTags(Entity_t& p_entity) const noexcept;

			/**
			 * @brief Check if the given entity contains any of the Tags given by parameter pack
			 * 
			 * @tparam Tags Types of the components
			 * @param p_entity Entity to check 
			 * @return constexpr uint8_t Whether the given entity contains the Tags given by parameter pack
			 */
			template<typename... Tags>
			constexpr uint8_t
			containsAnyTag(Entity_t& p_entity) const noexcept;

			/**
			 * @brief Remove all the components from all the entities
			 * 
			 * @param p_entity Entity to remove the components from
			 */
			constexpr void
			removeEntitiesComponents() noexcept;

			/**
			 * @brief Set the given entity to be destroyed at the end of the frame
			 * 
			 * @param p_entity Entity to be destroyed
			 */
			constexpr void
			setEntityForDestroy(Entity_t& p_entity) noexcept;

			/**
			 * @brief Remove all the entities from the entities container immediately.
			 * After the set for destruction for each entity an entity manager update will be called.
			 */
			constexpr void
			removeEntities() noexcept;

			/**
			 * @brief Create a copy of the given entity
			 * 
			 * @param p_entityID Entity ID
			 */
			constexpr void
			copyEntity(Entity_t& p_entity) noexcept;
	};
}

#include "EntityManager.tpp"

#include "Utility/Log/Log.hpp"

struct NameDebugComponent;

namespace Leaf::ecs
{
	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::EntityManager(const size_t p_numEntities)
	{
		entities_.reserve(p_numEntities);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename SigCmps, typename SigTags, 
			 typename EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::TagsFlagMatching TagsFlag>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	forAllMatching(auto&& p_function) noexcept
	{
		forAllMatching_impl<TagsFlag>(p_function, SigCmps{}, SigTags{});
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	
	template<typename SigCmpsLeft, typename SigTagsLeft, 
			 typename EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::TagsFlagMatching TagsFlagLeft,
			 typename SigCmpsRight, typename SigTagsRight, 
			 typename EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::TagsFlagMatching TagsFlagRight>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	forAllMatchingPairs(auto&& p_function) noexcept
	{
		forAllMatchingPairs_impl<TagsFlagLeft,TagsFlagRight>(p_function, SigCmpsLeft{},  SigTagsLeft{},
																		 SigCmpsRight{}, SigTagsRight{});
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::TagsFlagMatching TagsFlag,
			 typename... Cmps, typename... Tags>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	forAllMatching_impl(auto&& p_function, mp::TypeList<Cmps...>,
										   mp::TypeList<Tags...>) noexcept
	{
		for(Entity_t& entity : entities_)
		{
			const auto isMatching = isEntityMatchingTheSignature<TagsFlag>
									(entity, mp::TypeList<Cmps...>{},
											 mp::TypeList<Tags...>{});
			if(isMatching)
				p_function(entity, getComponent<Cmps>(entity)... );
		}
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>

	template<typename EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::TagsFlagMatching TagsFlagLeft,
			 typename EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::TagsFlagMatching TagsFlagRight,
			 typename... CmpsLeft,  typename... TagsLeft,
			 typename... CmpsRight, typename... TagsRight>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	forAllMatchingPairs_impl(auto&& p_function, mp::TypeList<CmpsLeft...>,
										   		mp::TypeList<TagsLeft...>,
												mp::TypeList<CmpsRight...>,
												mp::TypeList<TagsRight...>) noexcept
	{
		for(Entity_t& entityLeft : entities_)
		{
			const auto isMatchingLeft = isEntityMatchingTheSignature<TagsFlagLeft>
										(entityLeft, mp::TypeList<CmpsLeft...>{},
											 		 mp::TypeList<TagsLeft...>{});
			if(isMatchingLeft)
			{
				for(Entity_t& entityRight : entities_)
				{
					const auto isMatchingRight = isEntityMatchingTheSignature<TagsFlagRight>
												 (entityRight, mp::TypeList<CmpsRight...>{},
														  	   mp::TypeList<TagsRight...>{});
					if(isMatchingRight)
						p_function(entityLeft,  getComponent<CmpsLeft>(entityLeft)...,
								   entityRight, getComponent<CmpsRight>(entityRight)...);
				}
			}
		}
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::TagsFlagMatching TagsFlag,
	 		 typename... Cmps, typename... Tags>
	constexpr bool
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	isEntityMatchingTheSignature(Entity_t& entity, mp::TypeList<Cmps...>,
										   		   mp::TypeList<Tags...>) const noexcept
	{
		auto hasCmps = containsComponents<Cmps...>(entity);
		hasCmps = sizeof...(Cmps) > 0 ? hasCmps : 1;
		
		bool isMatching {true};
		if(TagsFlag == TagsFlagMatching::AllTagsRequired)
			isMatching = containsTags<Tags...>(entity) && hasCmps;
		else
			isMatching = ( ... || containsTags<Tags>(entity)) && hasCmps;
		
		isMatching = sizeof...(Tags) > 0 ? isMatching : ( 1 && hasCmps);

		return isMatching;	
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	update() noexcept
	{
		bool shouldRemoveEntities { false };

		for(Entity_t& entity : entities_)
		{
			if(entity.getID() == NON_VALID_ENTITY_ID)
			{
				removeComponentsFromEntity(CmpsList{},entity);
				shouldRemoveEntities = true;
			}
		}

		if(shouldRemoveEntities)
			removeDeadEntities();
	}


	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr typename EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::Entity_t&
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	createEntity() noexcept
	{
		return entities_.emplace_back();
	}

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	setEntityID(Entity_t& p_entity, size_t p_id) noexcept
	{
		p_entity.id_ = p_id;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr auto*
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	getEntityByID_impl(auto* self, const std::size_t p_id) noexcept
	{
		//LF_ENGINE_ASSERT_BREAK(self->entities_.size(), "There are no entities!");

		decltype(&self->entities_[0]) result { nullptr };

		for(auto& entity : self->entities_)
			if(entity.getID() == p_id) return &entity;

		LF_ENGINE_LOG_ERROR("There is no entity with the given ID {0}", p_id);
		return result;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr auto*
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	getEntityByName_impl(auto* self, const std::string_view& p_name) noexcept
	{
		//LF_ENGINE_ASSERT_BREAK(self->entities_.size(), "There are no entities!");

		decltype(&self->entities_[0]) result { nullptr };

		for(auto& entity : self->entities_)
		{
			if(self->template containsComponents<NameDebugComponent>(entity))
			{
				auto& nameDebug = self->template getComponent<NameDebugComponent>(entity);
				if(nameDebug.name == p_name.data()) return &entity;
			}
		}

		LF_ENGINE_LOG_ERROR("There is no entity with the given name {0}", p_name.data());
		return result;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
    template<typename CMP, typename... ConstructorParams>
	constexpr CMP&
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	addComponent(Entity_t& p_entity, ConstructorParams&&... p_params) noexcept
	{
		if(containsComponents<CMP>(p_entity))
		{
			LF_ENGINE_LOG_WARN("Trying to add a duplicate component. Returning the existing component");

			return getComponent<CMP>(p_entity);
		}

		auto [slotKey, cmp] = componentStorage_.template addComponent<CMP>(p_params...);
		constexpr auto cmpMask = ComponentStorage_t::cmpsinfo::template getSingleListTypeMask<CMP>();
		p_entity.template addComponent<cmpMask>(slotKey);
		
		return *cmp;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename CMP>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	removeComponent(Entity_t& p_entity) noexcept
	{
		//LF_ENGINE_ASSERT_BREAK(containsComponents<CMP>(p_entity),
		//					   "Trying to remove a non-valid component from the entity");

		callResetIfPresent(&getComponent<CMP>(p_entity));

		auto slotKey = p_entity.template getComponent<CMP>();
		componentStorage_.template removeComponent<CMP>(slotKey);

		constexpr auto cmpMask = ComponentStorage_t::cmpsinfo::template getSingleListTypeMask<CMP>();
		p_entity.template removeComponent<cmpMask>();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename CMP>
	constexpr decltype(auto)
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	getComponent_impl(auto* self, const Entity_t& p_entity) noexcept
	{
		//LF_ENGINE_ASSERT_BREAK(self->template containsComponents<CMP>(p_entity),
		//					   "Trying to get a non valid component from an entity");

		auto slotKey = p_entity.template getComponent<CMP>();
		
		return self->componentStorage_.template getComponent<CMP>(slotKey); 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename SingletonCMP>
	constexpr auto&
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	getSingletonComponent_impl(auto* self) noexcept
	{
		return self->componentStorage_.template getSingletonComponent<SingletonCMP>();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename CMP>
	constexpr decltype(auto)
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	getComponents_impl(auto* self) noexcept
	{
		return self->componentStorage_.template getComponents<CMP>();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename... CMPs>
	constexpr bool
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	containsComponents(const Entity_t& p_entity) const noexcept
	{
		constexpr auto cmpsMask = cmpsinfo::template getListTypesMask<CMPs...>();
		return p_entity.template containsComponents<cmpsMask>();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
    template<typename... Tags>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	addTags(Entity_t& p_entity) const noexcept
	{
		static_assert(sizeof...(Tags),"ERROR: Trying to add an empty list of tags to an entity");

		constexpr auto tagsMask = tagsinfo::template getListTypesMask<Tags...>();
		p_entity.template addTags<tagsMask>();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
    template<typename... Tags>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	removeTags(Entity_t& p_entity) const noexcept
	{
		static_assert(sizeof...(Tags),"ERROR: Trying to remove an empty list of tags to an entity");

		constexpr auto tagsMask = tagsinfo::template getListTypesMask<Tags...>();
		p_entity.template removeTags<tagsMask>();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
    template<typename... Tags>
	constexpr uint8_t
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	containsTags(Entity_t& p_entity) const noexcept
	{
		constexpr auto tagsMask = tagsinfo::template getListTypesMask<Tags...>();
		return p_entity.template containsTags<tagsMask>();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
    template<typename... Tags>
	constexpr uint8_t
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	containsAnyTag(Entity_t& p_entity) const noexcept
	{
		constexpr auto tagsMask = tagsinfo::template getListTypesMask<Tags...>();
		return p_entity.template containsAnyTag<tagsMask>();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template <typename CMP>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	removeComponentFromEntity(Entity_t& p_entity) noexcept
	{
		if(containsComponents<CMP>(p_entity))
			removeComponent<CMP>(p_entity);
	}	

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template <typename... CMPs>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	removeComponentsFromEntity(mp::TypeList<CMPs...>, Entity_t& p_entity) noexcept
	{
		(removeComponentFromEntity<CMPs>(p_entity), ...);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	removeEntitiesComponents() noexcept
	{
		for(auto& entity : entities_)
			removeComponentsFromEntity(CmpsList{}, entity);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	setEntityForDestroy(Entity_t& p_entity) noexcept
	{
		p_entity.id_ = NON_VALID_ENTITY_ID;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	removeDeadEntities() noexcept
	{
		entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [](const Entity_t& entity)
		{
			return entity.getID() == NON_VALID_ENTITY_ID;
		}), entities_.end());
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------


	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	removeEntities() noexcept
	{
		for(Entity_t& entity : entities_)
			removeComponentsFromEntity(CmpsList{}, entity);
		
		componentStorage_.clearStorage();
		entities_.clear();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename CMP>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	copyEntityComponent(Entity_t& p_entity, Entity_t& p_newEntity) noexcept
	{
		if(containsComponents<CMP>(p_entity))
		{
			auto& cmp = getComponent<CMP>(p_entity);
			addComponent<CMP>(p_newEntity, cmp);
		}
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	template<typename... CMPs, typename... TAGs>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	copyEntityComponentsAndTags(mp::TypeList<CMPs...>, mp::TypeList<TAGs...>,
								Entity_t& p_entity, Entity_t& p_newEntity) noexcept
	{
		auto fnTags = [&]<typename TAG>(TAG)
		{
			if(containsTags<TAG>(p_entity))
				addTags<TAG>(p_newEntity);
		};

		//Copying components
		( copyEntityComponent<CMPs>(p_entity, p_newEntity), ... );

		//Copying tags !! This is valid cause we are creating empty structs with tags.
		(fnTags(TAGs{}), ...);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	
	template<typename CmpsList, typename SingletonCmpsList, typename TagsList, size_t Capacity>
	constexpr void
	EntityManager<CmpsList,SingletonCmpsList,TagsList,Capacity>::
	copyEntity(Entity_t& p_entity) noexcept
	{
		//LF_ENGINE_ASSERT_BREAK(getEntityByID(p_entity.getID()),"Trying to copy a non valid entity!");

		auto newEntityName = getComponent<NameDebugComponent>(p_entity).name + "_copy";
		auto& newEntity = createEntity();

		copyEntityComponentsAndTags(CmpsList{}, TagsList{}, p_entity, newEntity);
		getComponent<NameDebugComponent>(newEntity).name = newEntityName;
	}	
}

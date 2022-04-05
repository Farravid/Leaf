namespace Leaf::ecs
{
	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	bool
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	operator==(const BaseEntity& p_entity) const noexcept
	{
		if(this != &p_entity)					return false;

		return true;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	
	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	constexpr std::size_t
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	getID() const noexcept
	{
		return id_;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	
	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	template<TypeMaskCmps CMPMask>
	constexpr void 
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	addComponent(auto slotKey) noexcept
	{
		components_ |= CMPMask;
		std::get<decltype(slotKey)>( componentKeys_ ) = slotKey;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	template<TypeMaskCmps CMPMask>
	constexpr void 
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	removeComponent() noexcept
	{
		components_ ^= (components_ & CMPMask);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	template<typename CMP>
	constexpr auto
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	getComponent() const noexcept
	{
		return std::get< key_type_t<CMP> >(componentKeys_);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	constexpr auto&
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	getComponentKeys() const noexcept
	{
		return componentKeys_;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	template<TypeMaskCmps CMPsMask>
	constexpr uint8_t 
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	containsComponents() const noexcept
	{
		return (components_ & CMPsMask) == CMPsMask;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	template<TypeMaskTags TAGsMask>
	constexpr void 
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	addTags() noexcept
	{
		tags_ |= TAGsMask;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	template<TypeMaskTags TAGsMask>
	constexpr void 
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	removeTags() noexcept
	{
		tags_ ^= (tags_ & TAGsMask);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	template<TypeMaskTags TAGsMask>
	constexpr bool 
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	containsTags() const noexcept
	{
		return (tags_ & TAGsMask) == TAGsMask;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename TypeMaskCmps, typename TypeMaskTags, typename CmpList, size_t Capacity>
	template<TypeMaskTags TAGsMask>
	constexpr bool 
	BaseEntity<TypeMaskCmps,TypeMaskTags,CmpList,Capacity>::
	containsAnyTag() const noexcept
	{
		return (tags_ & TAGsMask) != 0;
	}
	
}


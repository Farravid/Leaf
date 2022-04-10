namespace Leaf::ecs
{
	template <typename CmpsList, typename SingletonCmpsList, std::size_t Capacity>
	template<typename CMP, typename... ConstructorParams>
	constexpr auto
	ComponentStorage<CmpsList,SingletonCmpsList,Capacity>::
	addComponent(ConstructorParams&&... p_params) noexcept
	{
		auto& slotMap = std::get< StorageType<CMP> >(components_);
		auto key = slotMap.push_back(CMP{std::forward<ConstructorParams>(p_params)...});
	
		return std::tuple{key, &(slotMap[key])};
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template <typename CmpsList, typename SingletonCmpsList, std::size_t Capacity>
	template<typename CMP>
	constexpr void
	ComponentStorage<CmpsList,SingletonCmpsList,Capacity>::
	removeComponent(auto p_slotKey) noexcept
	{
		auto& slotMap = std::get<StorageType<CMP>>(components_);
		slotMap.erase(p_slotKey);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template <typename CmpsList, typename SingletonCmpsList, std::size_t Capacity>
	template<typename CMP>
	static constexpr auto&
	ComponentStorage<CmpsList,SingletonCmpsList,Capacity>::
	getComponent_impl(auto* self, auto p_slotKey) noexcept
	{
		auto& slotMap = std::get< StorageType<CMP> >( self->components_ );
		return slotMap[p_slotKey];
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template <typename CmpsList, typename SingletonCmpsList, std::size_t Capacity>
	template<typename SingletonCMP>
	constexpr auto&
	ComponentStorage<CmpsList,SingletonCmpsList,Capacity>::
	getSingletonComponent_impl(auto* self) noexcept
	{
		return std::get< SingletonCMP >( self->singletonComponents_ );
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template <typename CmpsList, typename SingletonCmpsList, std::size_t Capacity>
	template<typename CMP>
	constexpr auto&
	ComponentStorage<CmpsList,SingletonCmpsList,Capacity>::
	getComponents_impl(auto* self) noexcept
	{
		return std::get< StorageType<CMP> >( self->components_ );
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template <typename CmpsList, typename SingletonCmpsList, std::size_t Capacity>
	constexpr void
	ComponentStorage<CmpsList,SingletonCmpsList,Capacity>::
	clearStorage() noexcept
	{
		//TODO: Que pasa aqui nene
		//( std::get< StorageType<CmpsList> >(components_).clear(), ...);
	}
}
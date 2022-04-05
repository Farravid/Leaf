#include "Debug/Assertion.hpp"
#include "Debug/Log/Log.hpp"

namespace Leaf::ecs
{
	template<typename... SystemTypes>
	template<typename SysType, typename... ConstructorParams>
	constexpr auto& 
	SystemManager<SystemTypes...>::instantiate(ConstructorParams&&... p_params) noexcept
	{	
		auto sys = std::make_unique<SysType>(std::forward<ConstructorParams>(p_params)...);

		auto* result = sys.get();
		std::get<UniqPtr<SysType>>(systems_) = std::move(sys);

		return *result;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	template<typename... Systems>
	constexpr void
	SystemManager<SystemTypes...>::clearExcept() noexcept
	{
		OG_ENGINE_LOG_WARN(LOG_EXTENDED_OUTPUT,
		"Clearing systems. Take care of no deleting the render and input system!");
				
		auto fn = [](auto&& system)
		{
			const bool shouldDelete = ( true && (!std::is_same_v<Systems*, decltype(system.get())> && ...));
			if (system && shouldDelete)
				system.reset();
		};

		std::apply([&](auto&&... sys)
		{
			(fn(sys),...);
		}, systems_);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	template<typename... Systems>
	constexpr void
	SystemManager<SystemTypes...>::setActive(const bool p_isActive) noexcept
	{
		auto fn = [p_isActive](auto&& system)
		{
			if(system)
				system->isActive_ = p_isActive;
		};

		( fn(std::get<UniqPtr<Systems>>(systems_)), ... );
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	constexpr void
	SystemManager<SystemTypes...>::setActiveForAll(const bool p_isActive) noexcept
	{
		auto fn = [p_isActive](auto&& system)
		{
			if(system)
				system->isActive_ = p_isActive;
		};

		( fn(std::get<UniqPtr<SystemTypes>>(systems_)), ... );
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	template<typename SysType>
	constexpr SysType&
	SystemManager<SystemTypes...>::getSystem() const noexcept
	{
		return *(std::get<UniqPtr<SysType>>(systems_));
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	constexpr auto&
	SystemManager<SystemTypes...>::getSystems() const noexcept
	{
		return systems_;
	}
	
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	template<typename SysType>
	void	
	SystemManager<SystemTypes...>::initSystem(auto& p_application, SysType* system) const noexcept
	{	
		if(system && !system->isInitialized_)
			static_cast<SystemBase<SysType>*>(system)->init(p_application);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	void 
	SystemManager<SystemTypes...>::init(auto& p_application) const noexcept
	{
		std::apply([&](auto&&... p_args)
		{
			(initSystem(p_application, p_args.get()), ...);
		
		},systems_);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	template<typename TypeEntityTag, typename SysType>
	void	
	SystemManager<SystemTypes...>::updateSystem(auto& p_context, auto& p_application) const noexcept
	{	
		SysType* system { std::get<UniqPtr<SysType>>(systems_).get() };

		if(system)
		{
			if(system->isInitialized_ && system->isActive_)
				static_cast<SystemBase<SysType>*>(system)->template update<TypeEntityTag>(p_context, p_application);
			else
			{
				OG_ENGINE_LOG_WARN(LOG_SIMPLE_OUTPUT,
				"Trying to update and instanciated system but not initialized or active: {0}", system->name_);
			}
		}
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename... SystemTypes>
	template<typename TypeEntityTag>
	void 
	SystemManager<SystemTypes...>::update(auto& p_context, auto& p_application) const noexcept
	{
		(updateSystem<TypeEntityTag, SystemTypes>(p_context, p_application) , ...);
	}
}

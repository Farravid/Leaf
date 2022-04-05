namespace Leaf::ecs
{
	template <typename CRTPType>
	SystemBase<CRTPType>::SystemBase(const int p_frameRate, const std::string_view& p_name) noexcept
		: frameRate_ {p_frameRate}
		, name_ {p_name}
	{}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template <typename CRTPType>
	template<typename... SpecificSystemInitParams>
	void
	SystemBase<CRTPType>::init(SpecificSystemInitParams&&... p_params) noexcept
	{
		//!! If you get an error here, you probably forgot to define a init method on a ECS system.
		isInitialized_ = true;
		static_cast<CRTPType&>(*this).init(std::forward<SpecificSystemInitParams>(p_params)...);
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template <typename CRTPType>
	template<typename TypeEntityTag, typename... SpecificSystemUpdateParams>
	void
	SystemBase<CRTPType>::update(SpecificSystemUpdateParams&&... p_params) noexcept
	{
		if(!isActive_) return;
		
		double time = timer_.ellapsedSeconds();
		deltaTime_ 	= time - lastTime_;

		if(deltaTime_ >= (1.0f / frameRate_))
		{
			lastTime_ = time;
			
			static_cast<CRTPType&>(*this).template update<TypeEntityTag>
			(std::forward<SpecificSystemUpdateParams>(p_params)...);
		}
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template <typename CRTPType>
	float
	SystemBase<CRTPType>::getDeltaTime() const noexcept
	{
		return deltaTime_; 
	}
}


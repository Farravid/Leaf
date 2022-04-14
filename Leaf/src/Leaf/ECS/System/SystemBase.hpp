/**
 * @file SystemBase.hpp
 * @author OcachoGames-David (ocachogames@gmail.com)
 * @brief Base system for Ocacho Engine
 * 
 * All the systems handled by the SystemManager should derive from this class with CRTP
 * 
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "Time/Timer.hpp"

namespace Leaf::ecs
{
	template <typename CRTPType>
	class SystemBase
	{
		private:
			/**
			 * @brief Construct a new System Base object
			 * 
			 * @param p_frameRate Frame rate of the system
			 * @param p_name Name of the system
			 */
			explicit SystemBase(const uint8_t p_frameRate, const std::string_view& p_name) noexcept;

			//CRTP inheritance
			friend CRTPType;

		private:
			//Delta time of each system
			double   deltaTime_;
			//Last update time
			double 	 lastTime_{0.0f};
			//Timer to handle frame rate
			Timer    timer_;

		public:
			//Frame rate of each system
			uint8_t frameRate_;
			//Name of each system
			std::string name_{};
			//Whether the system is active to loop or not
			bool isActive_ {false};
			//Whether the system is initialized or not
			bool isInitialized_ { false };

		public:

			/**
			 * @brief Call the crtptype init with the parameter pack
			 * 
			 * @tparam SpecificSystemInitParams Types of init parameters
			 * @param p_params Parameter pack with the variables for crtptype init
			 */
			template<typename... SpecificSystemInitParams>
			void
			init(SpecificSystemInitParams&&... p_params) noexcept;

			/**
			 * @brief Handle the frame rate logic and call the crtptype update with the parameter pack
			 * 
			 * @tparam SpecificSystemUpdateParams Types of update parameters
			 * @param p_params Parameter pack with the variables for crtptype update
			 */
			template<typename TypeEntityTag, typename... SpecificSystemUpdateParams>
			void 
			update(SpecificSystemUpdateParams&&... p_params) noexcept;

			/**
			 * @brief Get the Delta Time
			 * 
			 * @return float Delta time
			 */
			float
			getDeltaTime() const noexcept;
	};
}

#include "SystemBase.tpp"

/**
 * @file SystemManager.hpp
 * @author OcachoGames-David (ocachogames@gmail.com)
 * @brief Manager for our systems.
 * This class holds the manage to instantiate and loop through the game systems.
 * The class has an array of update function pointers in order to call all the updates
 * of each system. 
 * 
 * @version 0.1
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "Utility/TypeAliases.hpp"
#include "System/SystemBase.hpp"

namespace Leaf::ecs
{
	template<typename... SystemTypes>
	class SystemManager
	{
		private:
			//Tuple containing the systems
			std::tuple<UniqPtr<SystemTypes>...> systems_{};

		private:
			/**
			 * @brief Update each system casting it down to the system base
			 * in order to handle frame rate of each system
			 * 
			 * @tparam SysType Type of the sytem
			 * @param p_context Entity manager context
			 * @param system System itself
			 */
			template<typename TypeEntityTag, typename SysType>
			void updateSystem(auto& p_context, auto& p_application) const noexcept;

			/**
			 * @brief Init each system casting it down to the system base
			 * in order to handle frame rate of each system
			 * 
			 * @tparam SysType Type of the sytem
			 * @param p_application Application context
			 * @param system System itself
			 */
			template<typename SysType>
			void initSystem(auto& p_application, SysType* system) const noexcept;

		public:
			/**
			 * @brief Create the given SysType system with the given constructor params
			 * parameter pack. 
			 * 
			 * @tparam SysType Type of the system to build
			 * @tparam ConstructorParams Type of system construct params
			 * @param p_params Constructor params to intialize the system
			 * @return SysType& Created system as reference
			 */
			template<typename SysType, typename... ConstructorParams>
			constexpr auto& 
			instantiate(ConstructorParams&&... p_params) noexcept;

			/**
			 * @brief Clear all the systems except the specificied systems in the variadic template 
			 * 
			 * @tparam Systems Systems to NOT delete
			 */
			template<typename... Systems>
			constexpr void
			clearExcept() noexcept;

			/**
			 * @brief Set the current system status (ACTIVE OR NOT)
			 * in order to loop or not
			 * By default, the systems are deactivated.
			 * 
			 * @tparam Systems Types of the systems you want to change the status
			 * @param p_isActive The next status of the systems
			 */
			template<typename... Systems>
			constexpr void
			setActive(const bool p_isActive) noexcept;

			/**
			 * @brief Set the current systems status (ACTIVE OR NOT)
			 * in order to loop or not
			 * By default, the systems are deactivated.
			 * 
			 * @param p_isActive The next status of the systems
			 */
			constexpr void
			setActiveForAll(const bool p_isActive) noexcept;

			/**
			 * @brief Get the given SysType system
			 * 
			 * @tparam SysType Type of the system
			 * @return SysType& System as reference
			 */
			template<typename SysType>
			constexpr SysType&
			getSystem() const noexcept;

			/**
			 * @brief Get the systems
			 * 
			 * @return auto& Tuple of Systems as reference
			 */
			constexpr auto&
			getSystems() const noexcept;

			/**
			 * @brief Loop through all the created systems of the game 
			 * calling the init function for each of them with the given context
			 * 
			 * @param auto& Entity manager context
			 */
			void init(auto& p_application) const noexcept;

			/**
			 * @brief Loop through all the created systems of the game 
			 * calling the update function for each of them with the given context
			 * 
			 * @tparam TypeEntityTag Type of the current updated entities (Editor or Game)
			 * @param auto& Entity manager context
			 */
			template<typename TypeEntityTag>
			void update(auto& p_context, auto& p_application) const noexcept;
	};
}

#include "SystemManager.tpp"
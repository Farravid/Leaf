/**
 * @file Application.hpp
 * @author OcachoGames-David (ocachogames@gmail.com)
 * @brief Base application of Ocacho engine.
 *
 * Each game app should derive from Application.
 * To create a game the derived classes should implement ::createApplication
 * See NoGasRacingApp.cpp as an example
 * 
 * @version 0.1
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "Ocacho.hpp"

namespace Leaf
{
	template<typename CRTPType>
	class Application
	{
		private:
			Application() = default;
			friend CRTPType;

			/**
			 * @brief getEntityManager implementation. See description below
			*/
			static constexpr auto*
			getEntityManager_impl(auto* self) noexcept { return self->entityMan_ };
			
			/**
			 * @brief getSystemManager implementation. See description below
			 */
			static constexpr auto*
			getSystemManager_impl(auto* self) noexcept { return self->systemMan_ };

		protected:

			//Entity manager
			EntityManager_t entityMan_ 			{ };
			//System manager
			SystemManager_t systemMan_ 			{ };

		public:
			/**
			 * @brief Initialization of the Application 
			 */
			constexpr void 
			init() noexcept;

			/**
			* @brief Main loop of the application
			*/
			constexpr void
			run() noexcept;

			/**
			 * @brief Get the Entity Manager of the application
			 * 
			 * @return EntityManager_t& As reference
			 */
			const EntityManager_t&
			getEntityManager() const noexcept { return getEntityManager_impl(this); }
			EntityManager_t&
			getEntityManager() noexcept { return getEntityManager_impl(this); }

			/**
			 * @brief Get the System Manager of the application
			 * 
			 * @return SystemManager_t& As reference
			 */
			const SystemManager_t&
			getSystemManager() const noexcept { return getSystemManager_impl(this); }
			SystemManager_t&
			getSystemManager() noexcept { return getSystemManager_impl(this); }
			
	};
}

#include "Application.tpp"


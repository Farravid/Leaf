/**
 * @file SlotMap.hpp
 * @author Ocacho Games - Antonio (ocachogames@gmail.com)
 * @brief This class contains the interface for using the Slot Map data structure.
 * @version 0.1
 * @date 2022-02-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <Utility/TypeAliases.hpp>
#include <Ocacho/Debug/Log/Log.hpp>

namespace Leaf::DataStructures
{
	template<typename DATA_T, typename INDEX_T = uint32_t, size_t Capacity = 100>
	class SlotMap
	{
		public:
			//Type of the data that is going to be saved in the data_ array
			using value_type = DATA_T;
			//Type used for the index values
			using index_type = INDEX_T;
			//Type used for the generation values
			using gen_type	 = index_type;
			//Type used for the key values. The id points to the data_ and erase_ position corresponding to this slot.
			using key_type	 = struct {index_type id; gen_type gen; };

			//Iterators for the data_ array
			using iterator	= DATA_T*;
			using citerator = DATA_T const*;
		
		private:
			//Number that signals the first free slot in the slotMap
			index_type	freelist_{};
			//Number that signals the number of operations made. It is used as part of the key
			gen_type	generation_{};
			//Number that signals how much elements there are in the slotMap (data_ and erase_ elements)
			index_type	size_{};

			//Array to save the slots of the slotMap
			std::array<key_type, 	Capacity> slots_{};
			//Array to save the data elements
			std::array<value_type, 	Capacity> data_{};
			//Array that keeps the ids of the slots in order to have control on which slot needs to be updated/delted
			std::array<index_type, 	Capacity> erase_{};

		public:
			/**
			 * @brief Constructor of the slotMap. Initializes the slotMap with the provided Capacity.
			 * 
			 */
			explicit constexpr SlotMap();

		private:
			//=========================================================================
			//PRIVATE METHODS 
			//=========================================================================

			/**
			 * @brief IniInitializes the slotMap with the provided Capacity. 
			 * Initializes the keys of each slot.
			 * 
			 */
			constexpr void freelist_init() noexcept;

			/**
			 * @brief Frees the slot indicated by p_key.
			 * Also sorts the data_ and erase_ arrays and updates de the size_
			 * and generation_ values.
			 * 
			 * @param p_key Key used to find the slot we want to free
			 */
			constexpr void free(key_type p_key) noexcept;

			/**
			 * @brief When deleting an slot, this method copies the last element
			 * in the data_ and erase_ arrays to the first free spot if the erased element wasn't
			 * the last in these arrays.
			 * It also updates de the id of the slot affected by this resort.
			 * 
			 * @param p_id Position of the data_ and erase_ arrays that are going to be updated
			 */
			constexpr void copy_last(index_type p_id) noexcept;

			/**
			 * @brief Checks if a key is valid.
			 * 
			 * @param p_key Key to be checked
			 * @return Returns true if is valid, false otherwise. 
			 */
			constexpr uint8_t is_valid(key_type p_key) const noexcept;

			/**
			 * @brief Method that reserves and initializes a slot.
			 * The slot id and gen are initilized.
			 * The freelist_, size_ and generation_ are updated.
			 * 
			 * @return Returns the position where the new slot is allocated in the slots_ array.
			 */
			[[nodiscard]] constexpr index_type allocate();

		public:
			//=========================================================================
			//PUBLIC METHODS 
			//=========================================================================

			/**
			 * @brief Iterator to the first element of the data_ array
			 * 
			 * @return constexpr iterator 
			 */
			[[nodiscard]] constexpr iterator begin() noexcept;

			/**
			 * @brief Const Iterator to the first element of the data_ array
			 * 
			 * @return constexpr iterator 
			 */
			[[nodiscard]] constexpr citerator begin() const noexcept;

			/**
			 * @brief Const iterator to the first element of the data_ array
			 * 
			 * @return constexpr citerator 
			 */
			[[nodiscard]] constexpr citerator cbegin() const noexcept;

			/**
			 * @brief Iterator to the last element of the data_ array
			 * 
			 * @return constexpr iterator 
			 */
			[[nodiscard]] constexpr iterator end() noexcept;

			/**
			 * @brief Const Iterator to the last element of the data_ array
			 * 
			 * @return constexpr iterator 
			 */
			[[nodiscard]] constexpr citerator end() const noexcept;

			/**
			 * @brief Const iterator to the last element of the data_ array
			 * 
			 * @return constexpr citerator 
			 */
			[[nodiscard]] constexpr citerator cend() const noexcept;

			/**
			 * @brief Returns a referece of the component (or value) saved in the data_ array
			 * managed by the slot refferred by the key used as parameter. 
			 * 
			 * @param p_key Key used to find the slot we want
			 * @return constexpr value_type& 
			 */
			[[nodiscard]] constexpr value_type& operator[](key_type p_key) noexcept;
			
			/**
			 * @brief Returns a const referece of the component (or value) saved in the data_ array
			 * managed by the slot refferred by the key used as parameter.
			 * This method will be called when the SlotMap is declared const in its instantiation.
			 * 
			 * @param p_key Key used to find the slot we want
			 * @return constexpr value_type const& 
			 */
			[[nodiscard]] constexpr value_type const& operator[](key_type p_key) const noexcept;
			
			/**
			 * @brief Returns the number of elements in the slotMap
			 * 
			 * @return constexpr index_type 
			 */
			[[nodiscard]] constexpr index_type size() const noexcept;

			/**
			 * @brief Returns the capacity of the slotMap
			 * 
			 * @return constexpr size_t 
			 */
			[[nodiscard]] constexpr size_t capacity() const noexcept;

			/**
			 * @brief Method used to insert an element in the slotMap.
			 * 
			 * @param p_data (Temporal value) Data element that is going to be saved in the data_ array
			 * @return Returns the key generated to manage the slot added.
			 */
			[[nodiscard]] constexpr key_type push_back(value_type&& p_data);

			/**
			 * @brief Method used to insert an element in the slotMap.
			 * 
			 * @param p_data (Reference) Data element that is going to be saved in the data_ array
			 * @return Returns the key generated to manage the slot added.
			 */
			[[nodiscard]] constexpr key_type push_back(value_type const& p_data);

			/**
			 * @brief Method used to clear the slots_ array and initilize again its slots.
			 * 
			 */
			constexpr void clear() noexcept;

			/**
			 * @brief Method used to erase an slot element from the slotMap
			 * 
			 * @param p_key Key used to find the slot we want to erase.
			 * @return Returns true if the delete action is correct, false otherwise.
			 */
			constexpr uint8_t erase(key_type p_key) noexcept;
	};
}

#include "SlotMap.tpp"
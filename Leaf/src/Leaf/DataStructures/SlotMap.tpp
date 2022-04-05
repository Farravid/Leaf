#include "Debug/Assertion.hpp"

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

namespace Leaf::DataStructures
{
	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr SlotMap<DATA_T, INDEX_T, Capacity>::SlotMap()
	{
		freelist_init();
	}

	//=========================================================================
	//PRIVATE METHODS 
	//========================================================================

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr void 
	SlotMap<DATA_T, INDEX_T, Capacity>::freelist_init() noexcept
	{
		for(index_type i{}; i<slots_.size() ; ++i)
		{
			slots_[i].id = i+1;
			slots_[i].gen = 0;
		}
		
		freelist_ 	= 0;
		size_ 		= 0;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr void 
	SlotMap<DATA_T, INDEX_T, Capacity>::free(key_type p_key) noexcept
	{
		OG_ENGINE_ASSERT(is_valid(p_key), "Invalid Key to free!");

			//Free the memory of the slots_ position
			auto& slot = slots_[p_key.id];

			//Check if we need to copy the last element to the earliest free spot of data_
			if(slot.id != size_ - 1)
				copy_last(slot.id);

			//The id of this slot points where freelist_ pointed before
			slot.id = freelist_;

			//Freelist value is now the slot position that has been freed
			freelist_ = p_key.id;

			//Update the generation to invalidate de key
			slot.gen = generation_;

			--size_;
			++generation_;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr void 
	SlotMap<DATA_T, INDEX_T, Capacity>::copy_last(index_type p_id) noexcept
	{
		data_[p_id] = data_[size_-1];
		erase_[p_id] = erase_[size_-1];
		slots_[erase_[p_id]].id = p_id; 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr uint8_t 
	SlotMap<DATA_T, INDEX_T, Capacity>::is_valid(key_type p_key) const noexcept
	{
		if(p_key.id >= Capacity 
				|| p_key.gen != slots_[p_key.id].gen)
				return false;

		return true;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::index_type 
	SlotMap<DATA_T, INDEX_T, Capacity>::allocate()
	{
		//1.- Check if there are free slots
		OG_ENGINE_ASSERT_BREAK(freelist_ < Capacity, "Error. Wrong control of freelist_");

		//2.- Reserve the next free slot & update the freelist_ value
		auto slot_pos = freelist_;
		freelist_ = slots_[slot_pos].id;

		//3.- Initialize the reserved slot
		auto& slot = slots_[slot_pos];
		slot.id = size_;
		slot.gen = generation_;

		//4.- Update the value of size and generation
		++size_;
		++generation_;

		return slot_pos;
	}

	//=========================================================================
	//PUBLIC METHODS 
	//=========================================================================

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::iterator
	SlotMap<DATA_T, INDEX_T, Capacity>::begin() noexcept
	{
		return data_.begin(); 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::citerator
	SlotMap<DATA_T, INDEX_T, Capacity>::begin() const noexcept
	{
		return data_.begin(); 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::citerator
	SlotMap<DATA_T, INDEX_T, Capacity>::cbegin() const noexcept
	{
		return data_.cbegin(); 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::iterator
	SlotMap<DATA_T, INDEX_T, Capacity>::end() noexcept
	{
		return data_.begin()+size_; 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::citerator
	SlotMap<DATA_T, INDEX_T, Capacity>::end() const noexcept
	{
		return data_.begin()+size_; 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::citerator
	SlotMap<DATA_T, INDEX_T, Capacity>::cend() const noexcept
	{
		return data_.cbegin()+size_; 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::value_type&
	SlotMap<DATA_T, INDEX_T, Capacity>::operator[](const key_type p_key) noexcept
	{
		//TODO: [Anto] Cuando haya tiempo, implementar la version cosnt y no const bien , sin repetir el codigo.--> PREGUNTAR A FRAN
		OG_ENGINE_ASSERT_BREAK(is_valid(p_key), "Invalid Key! Trying to access an invalid position...");
		return data_[slots_[p_key.id].id];
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::value_type const&
	SlotMap<DATA_T, INDEX_T, Capacity>::operator[](const key_type p_key) const noexcept
	{	
		OG_ENGINE_ASSERT_BREAK(is_valid(p_key), "Invalid Key! Trying to access an invalid position...");
		return data_[slots_[p_key.id].id];
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::index_type 
	SlotMap<DATA_T, INDEX_T, Capacity>::size() const noexcept
	{
		return size_;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr size_t 
	SlotMap<DATA_T, INDEX_T, Capacity>::capacity() const noexcept
	{
		return Capacity;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::key_type 
	SlotMap<DATA_T, INDEX_T, Capacity>::push_back(value_type&& p_data)
	{
		//1.- Reserve an slot
		auto reserved_id {allocate()};
		auto& slot = slots_[reserved_id];
		
		//2.- Set the values of the data_ and erase_ positions related to the reserved slot
		data_[slot.id] = std::move(p_data);
		erase_[slot.id] = reserved_id;
		
		//3.-Return the key
		auto key {slot};
		key.id = reserved_id;

		return key;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr typename SlotMap<DATA_T, INDEX_T, Capacity>::key_type 
	SlotMap<DATA_T, INDEX_T, Capacity>::push_back(value_type const& p_data)
	{
		return push_back(value_type{p_data});
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr void
	SlotMap<DATA_T, INDEX_T, Capacity>::clear() noexcept
	{
		freelist_init();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DATA_T, typename INDEX_T, size_t Capacity>
	constexpr uint8_t
	SlotMap<DATA_T, INDEX_T, Capacity>::erase(key_type p_key) noexcept
	{
		//1.- Check if the key is valid
		if(!is_valid(p_key))
			return false;

		free(p_key);
	
		return true;
	}
}

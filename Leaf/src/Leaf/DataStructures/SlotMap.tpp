#include "Utility/Log/Log.hpp"

namespace Leaf::containers
{
	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr SlotMap<DataType, Capacity>::SlotMap()
	{
		init();
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr void 
	SlotMap<DataType, Capacity>::init() noexcept
	{
		for(index_type i{}; i < Capacity; ++i)
			slots_[i].id = i + 1;

		freelist_ 	= 0;
		size_ 		= 0;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr void 
	SlotMap<DataType, Capacity>::free(key_type p_key) noexcept
	{
		//LF_ENGINE_ASSERT(is_valid(p_key), "Invalid Key to free!");

		auto& slot = slots_[p_key.id];

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

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr void 
	SlotMap<DataType, Capacity>::copy_last(index_type p_id) noexcept
	{
		data_[p_id] = data_[size_-1];
		erase_[p_id] = erase_[size_-1];
		slots_[erase_[p_id]].id = p_id; 
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr uint8_t 
	SlotMap<DataType, Capacity>::is_valid(key_type p_key) const noexcept
	{
		const bool isGenDifferent = p_key.gen != slots_[p_key.id].gen;
		const bool isKeyIDBiggerThanCapacity = p_key.id >= Capacity;

		return (isGenDifferent || isKeyIDBiggerThanCapacity) ? false : true;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr typename SlotMap<DataType, Capacity>::index_type 
	SlotMap<DataType, Capacity>::allocate()
	{
		if (size_ >= Capacity) std::out_of_range("Slotmap size bigger than Capacity");
		//LF_ENGINE_ASSERT(freelist_ < Capacity);

		//Reserve slot
		auto slotID = freelist_;
		freelist_ = slots_[slotID].id;

		//Init slot
		auto& slot = slots_[slotID];
		slot.id = size_;
		slot.gen = generation_;

		++size_;
		++generation_;

		return slotID;
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr typename SlotMap<DataType, Capacity>::value_type&
	SlotMap<DataType, Capacity>::operator[](const key_type p_key) noexcept
	{
		//TODO: Implement nice asserts
		LF_ENGINE_ASSERT(is_valid(p_key), "Invalid Key! Trying to access an invalid position");

		auto& slotID = slots_[p_key.id].id;
		return data_[slotID];
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr typename SlotMap<DataType, Capacity>::value_type const&
	SlotMap<DataType, Capacity>::operator[](const key_type p_key) const noexcept
	{
		//TODO: Implement nice asserts
		LF_ENGINE_ASSERT(is_valid(p_key), "Invalid Key! Trying to access an invalid position");

		auto& slotID = slots_[p_key.id].id;
		return data_[slotID];
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr typename SlotMap<DataType, Capacity>::key_type 
	SlotMap<DataType, Capacity>::push_back(value_type&& p_data)
	{
		auto reserved_id { allocate() };
		auto& slot = slots_[reserved_id];
		
		data_[slot.id] = std::move(p_data);
		erase_[slot.id] = reserved_id;
		
		return key_type{ slot.id, slot.gen };
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr typename SlotMap<DataType, Capacity>::key_type 
	SlotMap<DataType, Capacity>::push_back(value_type const& p_data)
	{
		return push_back( value_type{p_data} );
	}

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	template<typename DataType, size_t Capacity>
	requires Copyable<DataType>
	constexpr bool
	SlotMap<DataType, Capacity>::erase(key_type p_key) noexcept
	{
		if(!is_valid(p_key)) return false;

		free(p_key);
		return true;
	}
}
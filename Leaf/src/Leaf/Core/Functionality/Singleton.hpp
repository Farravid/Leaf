#pragma once

#include "Utility/Aliases/TypeAliases.hpp"
#include "NonCopyable.hpp"

namespace Leaf::func
{
	template <typename T>
	struct Singleton : NonCopyable
	{
		protected:
			static inline UniqPtr<T> instance_ { nullptr };

		public:
			static T& getInstance() noexcept
			{
				instance_ = instance_ ? instance_ : std::make_unique<T>();
				return *instance;
			}
	};
}

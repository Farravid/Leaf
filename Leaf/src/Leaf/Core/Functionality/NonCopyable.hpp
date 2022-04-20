#pragma once

namespace Leaf::func
{
	struct NonCopyable
	{
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable(const NonCopyable&&) = delete;
		NonCopyable& operator =(const NonCopyable&) = delete;
		NonCopyable& operator =(const NonCopyable&&) = delete;
	};
}

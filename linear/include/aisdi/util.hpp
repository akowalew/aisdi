#pragma once

#include <cassert>
#include <memory>

namespace aisdi {
namespace util {

template<class T>
void destroy_at(T* p)
{
    p->~T();
}

template<class ForwardIt>
inline void
destroy(ForwardIt first, ForwardIt last)
{
	for (; first != last; ++first)
	{
		destroy_at(std::addressof(*first));
	}
}

} // namespace util
} // namespace aisdi

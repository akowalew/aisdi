#pragma once

#include <memory>

namespace aisdi {

template<typename T>
class List
{
public:
	List() = default;

	bool empty() const
	{
		return true;
	}
};

} // namespace aisdi
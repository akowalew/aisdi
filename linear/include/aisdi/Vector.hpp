#pragma once

#include <memory>

namespace aisdi {

template<typename T>
class Vector
{
public:
	Vector() = default;

	bool empty() const
	{
		return true;
	}
};

} // namespace aisdi
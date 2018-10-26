#include "catch2/catch.hpp"

#include <memory>

#include "aisdi/Car.hpp"

TEST_CASE("Memleaks will be tested", "[memleak]")
{
	using namespace aisdi;

	for(auto i = 0; i < 100; ++i)
	{
		new int;
	}
}

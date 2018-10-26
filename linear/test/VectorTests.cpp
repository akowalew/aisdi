#include <boost/test/unit_test.hpp>

#include "aisdi/Vector.hpp"

BOOST_AUTO_TEST_CASE(DefaultConstructedVectorShouldBeEmpty)
{
	using namespace aisdi;
	
	const auto vector = Vector<int>{};

    BOOST_CHECK(vector.empty());
}

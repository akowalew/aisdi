#include <boost/test/unit_test.hpp>

#include "aisdi/List.hpp"

BOOST_AUTO_TEST_CASE(DefaultConstructedListShouldBeEmpty)
{
	using namespace aisdi;
	
	List<int> list;

    BOOST_CHECK(list.empty());
}

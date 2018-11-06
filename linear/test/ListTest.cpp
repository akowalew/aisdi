#define BOOST_TEST_MODULE ListTest
#include <boost/test/unit_test.hpp>

#include "aisdi/List.hpp"

BOOST_AUTO_TEST_CASE(DefaultConstructedListShouldBeEmpty)
{
	using namespace aisdi;
	
	const auto list = List<int>{};
 
    BOOST_CHECK(list.empty());
}

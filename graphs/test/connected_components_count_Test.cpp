#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <sstream>

#include "aisdi/connected_components_count.hpp"

namespace {

BOOST_AUTO_TEST_SUITE(connected_components_count_Test)

BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenGettingCCC_ThenZeroIsReturned)
{
	const auto graph = aisdi::Graph{};
	const auto cc_count = connected_components_count(graph);
	BOOST_CHECK(cc_count == 0);
}

BOOST_AUTO_TEST_CASE(GivenConnectedGraph_WhenGettingCCC_ThenOneIsReturned)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 3);
	graph.add_edge(3, 0);

	const auto cc_count = connected_components_count(graph);
	BOOST_CHECK(cc_count == 1);
}

BOOST_AUTO_TEST_CASE(GivenNotConnectedGraph_WhenGettingCCC_ThenItsCountIsReturned)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 3);
	graph.add_edge(3, 0);

	graph.add_edge(4, 5);
	graph.add_edge(5, 6);
	graph.add_edge(6, 7);
	graph.add_edge(7, 4);

	graph.add_edge(8, 9);
	graph.add_edge(9, 10);
	graph.add_edge(10, 11);
	graph.add_edge(11, 8);

	const auto cc_count = connected_components_count(graph);
	BOOST_CHECK(cc_count == 3);
}

BOOST_AUTO_TEST_SUITE_END() // connected_components_count_Test

} // namespace

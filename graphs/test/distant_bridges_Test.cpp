#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <sstream>
#include <iostream>

#include "aisdi/distant_bridges.hpp"

namespace {

BOOST_AUTO_TEST_SUITE(distant_bridges_Test)

BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenGettingDB_ThenEmptyListIsReturned)
{
	auto graph = aisdi::Graph{};
	const auto d_bridges = distant_bridges(graph);
	BOOST_CHECK(d_bridges.empty());
}

BOOST_AUTO_TEST_CASE(GivenSingleVertexGraph_WhenGettingDB_ThenEmptyListIsReturned)
{
	auto graph = aisdi::Graph{};
	graph.add_vertex(0);
	const auto d_bridges = distant_bridges(graph);
	BOOST_CHECK(d_bridges.empty());
}

BOOST_AUTO_TEST_CASE(GivenSingleEdgeGraph_WhenGettingDB_ThenEmptyListIsReturned)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	const auto d_bridges = distant_bridges(graph);
	BOOST_CHECK(d_bridges.empty());
}

BOOST_AUTO_TEST_CASE(GivenNotConnectedGraph_WhenGettingDB_ThenEmptyListIsReturned)
{
	auto graph = aisdi::Graph{};
	graph.add_vertex(0);
	graph.add_vertex(1);
	graph.add_vertex(2);
	graph.add_vertex(3);
	const auto d_bridges = distant_bridges(graph);
	BOOST_CHECK(d_bridges.empty());
}

BOOST_AUTO_TEST_CASE(GivenGraphWithoutDB_WhenGettingDB_ThenEmptyListIsReturned)
{
	{
		auto graph = aisdi::Graph{};
		graph.add_edge(0, 1);
		graph.add_edge(1, 2);
		graph.add_edge(2, 3);
		graph.add_edge(3, 0);

		const auto d_bridges = distant_bridges(graph);
		BOOST_CHECK(d_bridges.empty());
	}
}

BOOST_AUTO_TEST_SUITE_END() // distant_bridges_Test

} // namespace

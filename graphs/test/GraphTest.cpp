#include <boost/test/unit_test.hpp>

#include <algorithm>

#include <boost/mpl/list.hpp>

#include "aisdi/Graph.hpp"

namespace {

BOOST_AUTO_TEST_SUITE(GraphTest)

BOOST_AUTO_TEST_CASE(GivenGraph_WhenDefaultInitialized_ThenItShouldBeEmpty)
{
    const auto graph = aisdi::Graph{};
    const auto edges = graph.edges();
    const auto vertices = graph.vertices();

    BOOST_CHECK(graph.empty());
	BOOST_CHECK(edges.first == edges.last);
	BOOST_CHECK(vertices.first == vertices.last);
}

BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenAddingVertices_ThenItShouldBeNotEmpty)
{
	auto graph = aisdi::Graph{};
	graph.add_vertex(0);
	graph.add_vertex(1);
	graph.add_vertex(2);

    const auto edges = graph.edges();
    const auto vertices = graph.vertices();

	BOOST_CHECK(!graph.empty());
	BOOST_CHECK(edges.first == edges.last);
	BOOST_CHECK(vertices.first != vertices.last);
}

BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenAddingEdges_ThenItShouldBeNotEmpty)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 0);

    const auto edges = graph.edges();
    const auto vertices = graph.vertices();

	BOOST_CHECK(!graph.empty());
	BOOST_CHECK(edges.first != edges.last);
	BOOST_CHECK(vertices.first != vertices.last);
}

BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenAddingVertex_ThenItShouldBeAdded)
{
	auto graph = aisdi::Graph{};

	bool added;
	std::tie(std::ignore, added) = graph.add_vertex(0);

	BOOST_CHECK(added);
	BOOST_CHECK(graph.has_vertex(0));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenAddingSameVertex_ThenItShouldNotBeAdded)
{
	auto graph = aisdi::Graph{};

	bool first_added;
	std::tie(std::ignore, first_added) = graph.add_vertex(0);
	BOOST_REQUIRE(first_added);
	BOOST_REQUIRE(graph.has_vertex(0));

	bool second_added;
	std::tie(std::ignore, second_added) = graph.add_vertex(0);
	BOOST_CHECK(!second_added);
	BOOST_CHECK(graph.has_vertex(0));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenAddingNewVertex_ThenItShouldHaveZeroDegree)
{
	auto graph = aisdi::Graph{};

	graph.add_vertex(0);
	BOOST_REQUIRE(graph.has_vertex(0));
	BOOST_CHECK(graph.vertex_degree(0) == 0);
}

BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenAddingEdge_ThenItShouldAddNewEdge)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	BOOST_CHECK(graph.has_edge(0, 1));
}

BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenAddingEdge_ThenItShouldAddNewVertices)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	BOOST_CHECK(graph.has_vertex(0));
	BOOST_CHECK(graph.has_vertex(1));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenAddingEdgeToNewVertices_ThenItShouldAddNewEdges)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(0, 2);
	graph.add_edge(0, 3);

	BOOST_REQUIRE(graph.has_edge(0, 1));
	BOOST_CHECK(graph.has_edge(0, 2));
	BOOST_CHECK(graph.has_edge(0, 3));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenAddingEdgeToNewVertices_ThenItShouldAddNewVertices)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(0, 2);
	graph.add_edge(0, 3);

	BOOST_REQUIRE(graph.has_vertex(0));
	BOOST_REQUIRE(graph.has_vertex(1));
	BOOST_CHECK(graph.has_vertex(2));
	BOOST_CHECK(graph.has_vertex(3));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenAddingEdgesToVertex_ThenItShouldHaveExactDegree)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(0, 2);
	graph.add_edge(0, 3);

	BOOST_REQUIRE(graph.vertex_degree(0) == 3);
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenAddingEdgeToExistingVertices_ThenItShouldBeAdded)
{
	auto graph = aisdi::Graph{};
	graph.add_vertex(0);
	graph.add_vertex(1);
	BOOST_REQUIRE(graph.has_vertex(0));
	BOOST_REQUIRE(graph.has_vertex(1));

	graph.add_edge(0, 1);
	BOOST_CHECK(graph.has_edge(0, 1));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenItHasSomeEdge_ThenItShouldHaveReversedOne)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 0);

	BOOST_REQUIRE(graph.has_edge(0, 1));
	BOOST_CHECK(graph.has_edge(1, 0));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenItHasNotSomeEdge_ThenItShoudlNotHaveReversedOne)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 0);

	BOOST_REQUIRE(!graph.has_edge(111, 222));
	BOOST_CHECK(!graph.has_edge(222, 111));
}

BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenAddingEdges_ThenItShouldContainThem)
{
	using Graph = aisdi::Graph;
	using Edge = Graph::Edge;

	const auto edgesList = {Edge{0, 1}, Edge{1, 2}, Edge{2, 0}};
	auto graph = Graph{};
	for(const auto& edge : edgesList)
	{
		graph.add_edge(edge);
	}

	const auto edges = graph.edges();
	BOOST_CHECK(std::equal(edges.first, edges.last,
						   edgesList.begin(), edgesList.end()));
	BOOST_CHECK(std::all_of(edgesList.begin(), edgesList.end(),
							[&graph](const auto& edge)
							{
								return graph.has_edge(edge.u, edge.v);
							}));
	BOOST_CHECK(graph.has_vertex(0));
	BOOST_CHECK(graph.has_vertex(1));
	BOOST_CHECK(graph.has_vertex(2));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenListInitialized_ThenItShouldHaveTheseItems)
{
	using Graph = aisdi::Graph;
	using Edge = Graph::Edge;

	const auto edges_list = {Edge{0, 1}, Edge{1, 2}, Edge{2, 0}};
	const auto graph = aisdi::Graph{edges_list};

	BOOST_CHECK(graph.has_edge(0, 1));
	BOOST_CHECK(graph.has_edge(1, 2));
	BOOST_CHECK(graph.has_edge(2, 0));
	BOOST_CHECK(graph.has_vertex(0));
	BOOST_CHECK(graph.has_vertex(1));
	BOOST_CHECK(graph.has_vertex(2));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenRemovingEdge_ThenItShouldBeRemoved)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 0);

	BOOST_REQUIRE(graph.has_edge(0, 1));
	BOOST_REQUIRE(graph.has_edge(1, 0));

	const auto removed = graph.remove_edge(0, 1);
	BOOST_CHECK(removed);
	BOOST_CHECK(!graph.has_edge(0, 1));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenRemovingNonExistingEdge_ThenNothingHappens)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 0);

	BOOST_REQUIRE(graph.has_edge(0, 1));
	BOOST_REQUIRE(graph.has_edge(1, 0));

	const auto removed = graph.remove_edge(111, 222);
	BOOST_CHECK(!removed);
	BOOST_CHECK(!graph.has_edge(111, 222));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenRemovingVertex_ThenItShouldBeRemoved)
{
	auto graph = aisdi::Graph{};
	graph.add_vertex(0);
	graph.add_vertex(1);
	graph.add_vertex(2);

	BOOST_REQUIRE(graph.has_vertex(0));
	const auto removed = graph.remove_vertex(0);
	BOOST_CHECK(removed);
	BOOST_CHECK(!graph.has_vertex(0));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenRemovingNonExistingVertex_ThenNothingHappend)
{
	auto graph = aisdi::Graph{};
	graph.add_vertex(0);
	graph.add_vertex(1);
	graph.add_vertex(2);

	BOOST_REQUIRE(!graph.has_vertex(111));
	const auto removed = graph.remove_vertex(111);
	BOOST_CHECK(!removed);
	BOOST_CHECK(!graph.has_vertex(111));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenRemovingVertexWithEdges_ThenAllOfThemShouldBeRemoved)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 0);

	BOOST_REQUIRE(graph.vertex_degree(0) > 0);
	graph.remove_vertex(0);

	BOOST_CHECK(!graph.has_vertex(0));
	BOOST_CHECK(!graph.has_edge(0, 1));
	BOOST_CHECK(!graph.has_edge(2, 0));
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenRemovingVertexWithEdges_ThenOnlyThemShouldBeRemoved)
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(1, 2);
	graph.add_edge(2, 0);

	BOOST_REQUIRE(graph.vertex_degree(0) > 0);
	graph.remove_vertex(0);
	BOOST_REQUIRE(!graph.has_vertex(0));
	BOOST_REQUIRE(!graph.has_edge(0, 1));
	BOOST_REQUIRE(!graph.has_edge(2, 0));

	BOOST_CHECK(graph.has_edge(1, 2));
	BOOST_CHECK(graph.has_vertex(1));
	BOOST_CHECK(graph.has_vertex(2));
}

BOOST_AUTO_TEST_SUITE_END() // HashMapTests, Fixture

} // namespace

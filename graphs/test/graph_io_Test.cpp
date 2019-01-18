#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <sstream>

#include "aisdi/graph_io.hpp"

namespace {

BOOST_AUTO_TEST_SUITE(graph_io_Test)

BOOST_AUTO_TEST_CASE(GivenEmptyStream_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GivenStreamWithoutSize_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	ss << "aaabbbccc\n";
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GivenStreamWithZeroSize_WhenLoadingGraph_ThenEmptyGraphIsReturned)
{
	auto ss = std::stringstream{};
	ss << "0\n";

	const auto graph = aisdi::load_graph(ss);
	BOOST_CHECK(graph.empty());
}

BOOST_AUTO_TEST_CASE(GivenStreamWithInvalidSize_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	ss << "0123\n";
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GivenStreamWithOnlyNonZeroSize_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	ss << "123\n";
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GivenStreamWithInvalidSourceVertex_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	ss << "123\nabcd\n";
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GivenStreamWithOnlySourceVertex_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	ss << "123\n12\n";
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GivenStreamWithInvalidTargetVertex_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	ss << "123\n12 abcd\n";
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GivenStreamWithNotEnoughEdges_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	ss << "123\n12 32\n";
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GivenStream_WhenLoadingGraph_ThenItIsReturned)
{
	auto ss = std::stringstream{};
	ss << "3\n0 1\n1 2\n2 0\n";

	const auto graph = aisdi::load_graph(ss);
	BOOST_CHECK(graph.has_edge(0, 1));
	BOOST_CHECK(graph.has_edge(1, 2));
	BOOST_CHECK(graph.has_edge(2, 0));
}

BOOST_AUTO_TEST_CASE(GivenStreamWithEOF_WhenLoadingGraph_ThenExceptionIsThrown)
{
	auto ss = std::stringstream{};
	ss << "3\n0 1\n1 2\n2 0";
	BOOST_CHECK_THROW(aisdi::load_graph(ss), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END() // graph_io_Test

} // namespace

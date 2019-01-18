#include "aisdi/graph_io.hpp"

#include <istream>
#include <vector>

namespace aisdi {

namespace {

constexpr auto ExceptionsMask = (std::ios_base::failbit
								 | std::ios_base::badbit
								 | std::ios_base::eofbit);

template<typename Stream>
class StreamExceptionsGuard
{
public:
	StreamExceptionsGuard(Stream& stream, std::ios_base::iostate except)
		:	stream_(stream)
		,	prev_except_(stream.exceptions())
	{
		stream_.exceptions(except);
	}

	~StreamExceptionsGuard()
	{
		stream_.exceptions(prev_except_);
	}

private:
	Stream& stream_;
	std::ios_base::iostate prev_except_;
};

std::size_t read_num_edges(std::istream& is)
{
	int num_edges;
	is >> num_edges;
	if(num_edges < 0)
	{
		throw std::runtime_error("Could not read graph: invalid number of edges");
	}

	return static_cast<std::size_t>(num_edges);
}

Graph::Edge read_edge(std::istream& is)
{
	int u;
	is >> u;
	if(u < 0)
	{
		throw std::runtime_error("Could not read graph: invalid vertex number");
	}

	int v;
	is >> v;
	if(v < 0)
	{
		throw std::runtime_error("Could not read graph: invalid vertex number");
	}

	return {static_cast<Graph::VertexDescriptor>(u),
			static_cast<Graph::VertexDescriptor>(v)};
}

std::vector<Graph::Edge> read_edges(std::istream& is, std::size_t num_edges)
{
	std::vector<Graph::Edge> edges;
	auto elapsed = num_edges;
	while(elapsed--)
	{
		edges.emplace_back(read_edge(is));
	}

	return edges;
}

} //

Graph load_graph(std::istream& is)
{
	StreamExceptionsGuard<std::istream> guard{is, ExceptionsMask};

	const auto num_edges = read_num_edges(is);
	const auto edges = read_edges(is, num_edges);
	return Graph{edges.begin(), edges.end()};
}

} // aisdi

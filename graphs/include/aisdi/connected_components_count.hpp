#pragma once

#include "aisdi/Graph.hpp"
#include "aisdi/Vector.hpp"

namespace aisdi {

namespace {

struct DummyType {};

} //

inline std::size_t connected_components_count(const Graph& graph)
{
	std::size_t count = 0;
	aisdi::Vector<Graph::VertexDescriptor> stack;
	aisdi::HashMap<Graph::VertexDescriptor, DummyType> visited;

	const auto vertices = graph.vertices();
	for(auto vertex_pos = vertices.first; vertex_pos != vertices.last; ++vertex_pos)
	{
		const auto u = vertex_pos->first;
		if(visited.contains(u))
		{
			continue;
		}

		visited[u] = {};
		stack.append(u);

		while(!stack.empty())
		{
			const auto v = stack.popFront();
			const auto& target = graph.get_vertex(v);
			for(const auto k : target.adjacents)
			{
				if(visited.contains(k))
				{
					continue;
				}

				visited[k] = {};
				stack.append(k);
			}
		}

		++count;
	};

	return count;
}

} // aisdi

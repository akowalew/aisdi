#pragma once

#include "aisdi/Graph.hpp"
#include "aisdi/Vector.hpp"
#include "aisdi/connected_components_count.hpp"

namespace aisdi {

inline Vector<Graph::Edge> distant_bridges(Graph& graph)
{
	Vector<Graph::Edge> result;
	const auto cc_count = connected_components_count(graph);

	const auto edges = graph.edges();
	std::for_each(edges.first, edges.last, [&](const auto& edge)
	{
		const auto u = edge.u;
		const auto source = graph.pop_vertex(u);
		const auto v = edge.v;
		const auto target = graph.pop_vertex(v);

		const auto new_cc_count = connected_components_count(graph);
		if((new_cc_count != cc_count) 
			&& (new_cc_count != 0))
		{
			result.append(edge);
		}

		for(const auto k : source.adjacents)
		{
			graph.add_edge(u, k);
		}

		for(const auto k : target.adjacents)
		{
			graph.add_edge(v, k);
		}
	});

	return result;
}

} // aisdi

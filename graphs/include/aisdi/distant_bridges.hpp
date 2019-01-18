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
	std::for_each(edges.first, edges.last,
		[&](const auto& edge)
		{
			const auto u = edge.u;
			const auto source = graph.pop_vertex(u);

			const auto v = edge.v;
			const auto target = graph.pop_vertex(v);

			const auto new_cc_count = connected_components_count(graph);
			if(new_cc_count != cc_count)
			{
				result.append(edge);
			}

			const auto add_u_result = graph.add_vertex(u);
			const auto source_pos = add_u_result.first;
			for(const auto v : source.adjacents)
			{
				graph.add_edge(source_pos, v);
			}

			const auto add_v_result = graph.add_vertex(v);
			const auto target_pos = add_v_result.first;
			for(const auto v : target.adjacents)
			{
				graph.add_edge(target_pos, v);
			}
		});

	return result;
}

} // aisdi

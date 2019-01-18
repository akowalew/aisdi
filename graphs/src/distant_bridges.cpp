#include "aisdi/distant_bridges.hpp"

int main()
{
	auto graph = aisdi::Graph{};
	graph.add_edge(0, 1);
	graph.add_edge(0, 2);
	graph.add_edge(0, 3);
	graph.add_edge(1, 2);
	graph.add_edge(1, 3);
	graph.add_edge(2, 3);
	const auto d_bridges = distant_bridges(graph);
}

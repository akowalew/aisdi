#pragma once

#include <algorithm>
#include <initializer_list>
#include <utility>

#include "aisdi/HashMap.hpp"
#include "aisdi/List.hpp"
#include "aisdi/Vector.hpp"

namespace aisdi {

class Graph
{
public:
	using VertexDescriptor = unsigned int;

	struct Vertex
	{
		using Adjacents = aisdi::Vector<VertexDescriptor>;

		Adjacents adjacents;

		void clear_adjacents()
		{
			adjacents.clear();
		}

		void add_adjacent(VertexDescriptor u)
		{
			adjacents.append(u);
		}

		void remove_adjacent(VertexDescriptor u)
		{
			adjacents.erase(std::remove(adjacents.begin(),
								        adjacents.end(),
								        u),
					        adjacents.end());
		}

		std::size_t degree() const
		{
			return adjacents.size();
		}
	};

	using Vertices = aisdi::HashMap<VertexDescriptor, Vertex>;
	using VertexIterator = Vertices::const_iterator;

	struct VerticesRange
	{
		VertexIterator first;
		VertexIterator last;
	};

	struct Edge
	{
		VertexDescriptor u;
		VertexDescriptor v;
	};

	using Edges = aisdi::Vector<Edge>;
	using EdgeIterator = Edges::const_iterator;

	struct EdgesRange
	{
		EdgeIterator first;
		EdgeIterator last;
	};

	Graph() = default;

	template<typename InputIt>
	Graph(InputIt first, InputIt last)
	{
		add_edges(first, last);
	}

	Graph(std::initializer_list<Edge> edges)
		:	Graph(edges.begin(), edges.end())
	{}

	EdgesRange edges() const
	{
		return {edges_begin(), edges_end()};
	}

	EdgeIterator edges_begin() const
	{
		return edges_.cbegin();
	}

	EdgeIterator edges_end() const
	{
		return edges_.cend();
	}

	VerticesRange vertices() const
	{
		return {vertices_begin(), vertices_end()};
	}

	VertexIterator vertices_begin() const
	{
		return vertices_.cbegin();
	}

	VertexIterator vertices_end() const
	{
		return vertices_.cend();
	}

	template<typename InputIt>
	void add_edges(InputIt first, InputIt last)
	{
		std::for_each(first, last, [this](const auto& e) {
			this->add_edge(e);
		});
	}

	EdgeIterator add_edge(VertexDescriptor u, VertexDescriptor v)
	{
		return add_edge(Edge{u, v});
	}

	EdgeIterator add_edge(Edge edge)
	{
		vertices_[edge.u].add_adjacent(edge.v);
		vertices_[edge.v].add_adjacent(edge.u);
		return edges_.insert(edges_end(), edge);
	}

	EdgeIterator find_edge(VertexDescriptor u, VertexDescriptor v) const
	{
		return std::find_if(edges_begin(), edges_end(),
							[u, v](const auto& edge)
							{
								return ((edge.u == u) && (edge.v == v))
									   || ((edge.u == v) && (edge.v == u));
							});
	}

	bool remove_edge(VertexDescriptor u, VertexDescriptor v)
	{
		const auto pos = find_edge(u, v);
		if(pos == edges_end())
		{
			return false;
		}

		erase_edge(pos);
		return true;
	}

	bool has_edge(VertexDescriptor u, VertexDescriptor v) const
	{
		return (find_edge(u, v) != edges_end());
	}

	EdgeIterator erase_edge(const EdgeIterator& edge_pos)
	{
		Expects(edge_pos != edges_end());
		const auto& edge = *edge_pos;

		auto& source = get_vertex_(edge.u);
		source.remove_adjacent(edge.v);
		auto& target = get_vertex_(edge.v);
		target.remove_adjacent(edge.u);

		return edges_.erase(edge_pos);
	}

	const Vertex& get_vertex(VertexDescriptor u) const
	{
		const auto pos = find_vertex(u);
		Expects(pos != vertices_end());
		return pos->second;
	}

	VertexIterator find_vertex(VertexDescriptor v) const
	{
		return vertices_.find(v);
	}

	bool has_vertex(VertexDescriptor v) const
	{
		return (find_vertex(v) != vertices_end());
	}

	std::size_t vertex_degree(VertexDescriptor v) const
	{
		return get_vertex(v).degree();
	}

	std::pair<VertexIterator, bool> add_vertex(VertexDescriptor v)
	{
		const auto result = vertices_.insert({v, Vertex{}});
		const auto& pos = result.first;
		const auto& inserted = result.second;
		return {pos, inserted};
	}

	bool remove_vertex(VertexDescriptor u)
	{
		const auto pos = find_vertex(u);
		if(pos == vertices_end())
		{
			return false;
		}

		erase_vertex(pos);
		return true;
	}

	VertexIterator erase_vertex(const VertexIterator& pos)
	{
		Expects(pos != vertices_end());
		clear_vertex(pos);
		return vertices_.erase(pos);
	}

	void clear_vertex(VertexDescriptor u)
	{
		clear_vertex(find_vertex(u));
	}

	void clear_vertex(const VertexIterator& source_pos)
	{
		Expects(source_pos != vertices_end());
		const auto u = source_pos->first;
		auto& source = const_cast<Vertex&>(source_pos->second);

		for(auto v : source.adjacents)
		{
			auto& target = get_vertex_(v);
			target.remove_adjacent(u);
			erase_edge_(find_edge(u, v));
		}

		source.clear_adjacents();
	}

	bool empty() const
	{
		return vertices_.empty();
	}

private:
	Vertex& get_vertex_(VertexDescriptor u)
	{
		return const_cast<Vertex&>(get_vertex(u));
	}

	EdgeIterator erase_edge_(const EdgeIterator& edge_pos)
	{
		Expects(edge_pos != edges_end());
		return edges_.erase(edge_pos);
	}

	Edges edges_;
	Vertices vertices_;
};

bool operator==(const Graph::Edge& lhs, const Graph::Edge& rhs)
{
	return (lhs.u == rhs.u) && (lhs.v == rhs.v);
}

bool operator!=(const Graph::Edge& lhs, const Graph::Edge& rhs)
{
	return !(lhs == rhs);
}

} // aisdi

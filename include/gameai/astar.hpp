#pragma once
#include <gameai/inverse_comparator.hpp>
#include <gameai/astar_vertex.hpp>

namespace gameai {
	/* generalization can sometimes be ugly */
	template<typename HeuristicF, typename VertexPosT, typename GraphT>
	void astar(const typename boost::graph_traits<GraphT>::vertices_size_type &src,
	           const typename boost::graph_traits<GraphT>::vertices_size_type &dst,
	           const GraphT &g) {
		typedef typename boost::graph_traits<GraphT>::vertex_descriptor VertexT;
		boost::unordered_map<typename boost::graph_traits<GraphT>::vertices_size_type, VertexT> vertex_map;

		BGL_FORALL_VERTICES_T(v, g, GraphT) {
			vertex_map.emplace(boost::get(boost::vertex_name, g, v), v);
		}

		/* a heap-based priority queue is used to retrive the lowest-cost open node
		 * hash-based sets are used to lookup nodes in O(1)
		 */
		boost::heap::priority_queue<astar_vertex<VertexT>, boost::heap::compare<inverse_comparator<astar_vertex<VertexT>>>> open_heap;
		boost::unordered_set<astar_vertex<VertexT>> open_set;
		boost::unordered_set<astar_vertex<VertexT>> closed_set;

		astar_vertex<VertexT> v{0, 0, src, vertex_map.at(src)};
		open_heap.emplace(v);
		open_set.emplace(v);

		while(!open_heap.empty()) {
			auto current = open_heap.top();
			open_heap.pop();
			open_set.erase(current);

			std::cout << boost::get(boost::vertex_name, g, current.vertex) << std::endl;

			BOOST_FOREACH(auto edge, boost::out_edges(current.vertex, g)) {
				auto target = boost::target(edge, g);
				auto n = boost::get(boost::vertex_name, g, target);
				if(n == dst) {
					std::cout << dst << std::endl;
					return;
				}

				auto w = boost::get(boost::edge_weight, g, edge);
				auto dist_to_end  = HeuristicF()(boost::get(VertexPosT(), g, target),
				                                 boost::get(VertexPosT(), g, vertex_map.at(dst)));
				astar_vertex<VertexT> next{current.base_cost + w, dist_to_end, n, target};

				auto open_find = open_set.find(next);
				if(open_find != open_set.end() && open_find->total_cost() < next.total_cost()) { continue; }

				auto closed_find = closed_set.find(next);
				if(closed_find != closed_set.end() && closed_find->total_cost() < next.total_cost()) { continue; }

				open_heap.emplace(next);
				open_set.emplace(next);
			}
			closed_set.emplace(current);
		}
	}
}

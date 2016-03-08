#pragma once
#include <gameai/inverse_comparator.hpp>
#include <gameai/astar_vertex.hpp>

namespace gameai {
	struct astar_pos_t { typedef boost::vertex_property_tag kind; };

	template<typename HeuristicF, typename GraphT> class astar {
	public:
		typedef GraphT graph_type;
		typedef typename boost::graph_traits<graph_type> graph_traits_type;
		typedef typename boost::graph_traits<graph_type>::vertex_descriptor vertex_type;
		typedef astar_vertex<vertex_type> astar_vertex_type;
	private:
		HeuristicF heuristic;
		const graph_type &g;
	public:
		astar(const graph_type &g) : g(g) {}

		void path(const typename graph_traits_type::vertices_size_type &src,
				  const typename graph_traits_type::vertices_size_type &dst) {
			boost::unordered_map<typename graph_traits_type::vertices_size_type, vertex_type> vertex_map;

			BGL_FORALL_VERTICES_T(v, g, graph_type) {
				vertex_map.emplace(boost::get(boost::vertex_name, g, v), v);
			}

			/* a heap-based priority queue is used to retrieve the lowest-cost open node
			 * the heap is a max heap so an inverse comparator is used to ensure O(1) retrieval
			 * hash-based sets are used to lookup nodes in O(1)
			 */
			boost::heap::priority_queue<astar_vertex_type, boost::heap::compare<inverse_comparator<astar_vertex_type>>> open_heap;
			boost::unordered_set<astar_vertex_type> open_set;
			boost::unordered_set<astar_vertex_type> closed_set;

			astar_vertex_type v{0, 0, src, vertex_map.at(src)};
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
					auto dist_to_end = heuristic(boost::get(astar_pos_t(), g, target),
					                             boost::get(astar_pos_t(), g, vertex_map.at(dst)));
					astar_vertex_type next{current.base_cost + w, dist_to_end, n, target};

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
	};
	template<typename HeuristicF, typename GraphT> astar<HeuristicF, GraphT> make_astar(const GraphT &g) {
		return astar<HeuristicF, GraphT>(g);
	}
}

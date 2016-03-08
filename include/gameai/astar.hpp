#pragma once
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/container/vector.hpp>
#include <boost/heap/priority_queue.hpp>
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

		boost::container::vector<boost::shared_ptr<astar_vertex_type>>
		path(const typename graph_traits_type::vertices_size_type &src,
		     const typename graph_traits_type::vertices_size_type &dst) {
			boost::unordered_map<typename graph_traits_type::vertices_size_type, vertex_type> vertex_map;

			BGL_FORALL_VERTICES_T(v, g, graph_type) {
				vertex_map.emplace(boost::get(boost::vertex_name, g, v), v);
			}

			/* a heap-based priority queue is used to retrieve the lowest-cost open node
			 * the heap is a max heap so an inverse comparator is used to ensure O(1) retrieval
			 * hash-based sets are used to lookup nodes in O(1)
			 */
			boost::heap::priority_queue<
				boost::shared_ptr<astar_vertex_type>,
				boost::heap::compare<inverse_shared_ptr_comparator<astar_vertex_type>>
			> open_heap;
			boost::unordered_set<boost::shared_ptr<astar_vertex_type>> open_set;
			boost::unordered_set<boost::shared_ptr<astar_vertex_type>> closed_set;

			auto v = boost::make_shared<astar_vertex_type>(0, 0, src, vertex_map.at(src));
			open_heap.emplace(v);
			open_set.emplace(v);

			boost::unordered_map<boost::shared_ptr<astar_vertex_type>,
			                     boost::shared_ptr<astar_vertex_type>> came_from;

			while(!open_heap.empty()) {
				auto current = open_heap.top();
				open_heap.pop();
				open_set.erase(current);

				auto n = boost::get(boost::vertex_name, g, current->vertex);
				if(n == dst) {
					return reconstruct_path(came_from, current);
				}

				BOOST_FOREACH(auto edge, boost::out_edges(current->vertex, g)) {
					auto target = boost::target(edge, g);

					auto w = boost::get(boost::edge_weight, g, edge);
					auto dist_to_end = heuristic(boost::get(astar_pos_t(), g, target),
					                             boost::get(astar_pos_t(), g, vertex_map.at(dst)));
					auto next = boost::make_shared<astar_vertex_type>(current->base_cost + w, dist_to_end, n, target);

					auto open_find = open_set.find(next);
					if(open_find != open_set.end() && (*open_find)->total_cost() < next->total_cost()) { continue; }

					auto closed_find = closed_set.find(next);
					if(closed_find != closed_set.end() && (*closed_find)->total_cost() < next->total_cost()) { continue; }

					came_from.emplace(next, current);
					open_heap.emplace(next);
					open_set.emplace(next);
				}
				closed_set.emplace(current);
			}
			return {};
		}

		boost::container::vector<boost::shared_ptr<astar_vertex_type>>
		reconstruct_path(boost::unordered_map<boost::shared_ptr<astar_vertex_type>,
		                                      boost::shared_ptr<astar_vertex_type>> came_from,
		                 boost::shared_ptr<astar_vertex_type> target) {
			boost::container::vector<boost::shared_ptr<astar_vertex_type>> path;
			path.emplace_back(target);
			while(came_from.find(target) != came_from.end()) {
				target = came_from.at(target);
				path.emplace_back(target);
			}
			return path;
		}
	};
	template<typename HeuristicF, typename GraphT> astar<HeuristicF, GraphT> make_astar(const GraphT &g) {
		return astar<HeuristicF, GraphT>(g);
	}
}

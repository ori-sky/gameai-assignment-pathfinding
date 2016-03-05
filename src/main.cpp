#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/heap/priority_queue.hpp>
#include <boost/graph/graphviz.hpp>
#include <gameai/point2.hpp>
#include <gameai/astar_vertex.hpp>

struct vertex_pos_t { typedef boost::vertex_property_tag kind; };
struct edge_label_t { typedef boost::edge_property_tag kind; };

typedef boost::property<vertex_pos_t, gameai::point2<int>> vertex_pos_p;
typedef boost::property<boost::vertex_name_t, boost::graph_traits<boost::adjacency_list<>>::vertices_size_type, vertex_pos_p> vertex_p;
typedef boost::property<boost::edge_weight_t, unsigned int> edge_p;
typedef boost::no_property graph_p;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_p, edge_p, graph_p> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;
typedef gameai::astar_vertex<vertex_t> astar_vertex_t;

template<typename T> class inverse_comparator {
public:
	bool operator()(const T &lhs, const T &rhs) const {
		return lhs > rhs;
	}
};

/* generalization can sometimes be ugly */
template<typename ...Ts>
void astar(const typename boost::graph_traits<boost::adjacency_list<Ts...>>::vertices_size_type &src,
           const typename boost::graph_traits<boost::adjacency_list<Ts...>>::vertices_size_type &dst,
           const boost::adjacency_list<Ts...> &g) {
	boost::unordered_map<typename boost::graph_traits<boost::adjacency_list<Ts...>>::vertices_size_type, vertex_t> vertex_map;

	BGL_FORALL_VERTICES(v, g, graph_t) {
		vertex_map.emplace(boost::get(boost::vertex_name, g, v), v);
	}

	/* a heap-based priority queue is used to retrive the lowest-cost open node
	 * hash-based sets are used to lookup nodes in O(1)
	 */
	boost::heap::priority_queue<astar_vertex_t, boost::heap::compare<inverse_comparator<astar_vertex_t>>> open_heap;
	boost::unordered_set<astar_vertex_t> open_set;
	boost::unordered_set<astar_vertex_t> closed_set;

	astar_vertex_t v{0, 0, src, vertex_map.at(src)};
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
			auto dist_to_end  = gameai::distance_squared(boost::get(vertex_pos_t(), g, target),
			                                             boost::get(vertex_pos_t(), g, vertex_map.at(dst)));
			astar_vertex_t next{current.base_cost + w, dist_to_end, n, target};

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

int main(int argc, char **argv) {
	std::ifstream in("assets/graph.dot");

	graph_t g;
	boost::dynamic_properties dp(boost::ignore_other_properties);
	dp.property("node_id", boost::get(boost::vertex_name, g));
	dp.property("pos", boost::get(vertex_pos_t(), g));
	dp.property("label", boost::get(boost::edge_weight, g));

	boost::read_graphviz(in, g, dp);
	astar(1, 3, g);

	return 0;
}

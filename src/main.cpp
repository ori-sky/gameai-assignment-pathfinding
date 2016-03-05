#include <boost/unordered_map.hpp>
#include <boost/heap/priority_queue.hpp>
#include <boost/graph/graphviz.hpp>
#include <gameai/point2.hpp>

struct vertex_pos_t { typedef boost::vertex_property_tag kind; };
struct edge_label_t { typedef boost::edge_property_tag kind; };

typedef boost::property<vertex_pos_t, gameai::point2<unsigned int>> vertex_pos_p;
typedef boost::property<boost::vertex_name_t, unsigned int, vertex_pos_p> vertex_p;
typedef boost::property<boost::edge_weight_t, unsigned int> edge_p;
typedef boost::no_property graph_p;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_p, edge_p, graph_p> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;

/*class inverse_comparator {
public:
	bool operator()(const vertex_t &lhs, const vertex_t &rhs) {
		return boost::get(vertex_pos_t(), g, lhs > boost::get(vertex_pos_t(), g, rhs);
	}
};*/

int main(int argc, char **argv) {
	std::ifstream in("assets/graph.dot");

	graph_t g;
	boost::dynamic_properties dp(boost::ignore_other_properties);
	dp.property("node_id", boost::get(boost::vertex_name, g));
	dp.property("pos", boost::get(vertex_pos_t(), g));
	dp.property("label", boost::get(boost::edge_weight, g));

	boost::read_graphviz(in, g, dp);

	boost::unordered_map<int, vertex_t> vertex_map;
	BGL_FORALL_VERTICES(v, g, graph_t) {
		vertex_map.emplace(boost::get(boost::vertex_name, g, v), v);
	}

	//boost::heap::priority_queue<vertex_t, boost::heap::compare<inverse_comparator>> open;
	//boost::heap::priority_queue<vertex_t, boost::heap::compare<inverse_comparator>> closed;
	//open.emplace(vertex_map[1]);

	std::cout << boost::get(boost::vertex_name, g, vertex_map[1]) << std::endl;
	BOOST_FOREACH(auto edge, boost::out_edges(vertex_map[1], g)) {
		auto target = boost::target(edge, g);
		std::cout << "|_" << boost::get(boost::vertex_name, g, target) << std::endl;
	}

	return 0;
}

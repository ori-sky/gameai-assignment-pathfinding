#include <boost/graph/graphviz.hpp>

struct vertex_pos_t {
	typedef boost::vertex_property_tag kind;
};

struct edge_label_t {
	typedef boost::edge_property_tag kind;
};

typedef boost::property<vertex_pos_t, std::string> vertex_pos_p;
typedef boost::property<boost::vertex_name_t, std::string, vertex_pos_p> vertex_p;
typedef boost::property<boost::edge_weight_t, int> edge_p;
typedef boost::no_property graph_p;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_p, edge_p, graph_p> graph_t;

int main(int argc, char **argv) {
	std::ifstream in("assets/graph.dot");

	graph_t g;
	boost::dynamic_properties dp(boost::ignore_other_properties);
	dp.property("node_id", boost::get(boost::vertex_name, g));
	dp.property("pos", boost::get(vertex_pos_t(), g));
	dp.property("label", boost::get(boost::edge_weight, g));

	boost::read_graphviz(in, g, dp);

	boost::write_graphviz_dp(std::cout, g, dp);

	return 0;
}

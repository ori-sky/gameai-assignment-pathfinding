#include <fstream>
#include <sstream>
#include <gameai/DOT/tokenizer.hpp>

int main(int argc, char **argv) {
	using namespace gameai;

	std::ifstream in("assets/graph.dot");
	std::basic_ostringstream<DOT::token> out;
	DOT::tokenizer t(in, out);

	if(!in) { throw std::runtime_error("failed to open graph"); }

	return 0;
}

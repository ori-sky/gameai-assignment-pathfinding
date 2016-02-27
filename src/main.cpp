#include <fstream>
#include <sstream>
#include <gameai/DOT/tokenizer.hpp>
#include <gameai/DOT/parser.hpp>

int main(int argc, char **argv) {
	using namespace gameai;

	DOT::tokenizer<std::ifstream> tokenizer("assets/graph.dot");
	DOT::parser parser(tokenizer);

	return 0;
}

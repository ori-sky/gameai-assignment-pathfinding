#pragma once
#include <iostream>
#include <gameai/DOT/token.hpp>

namespace gameai {
	namespace DOT {
		class tokenizer {
		private:
			std::istream &in;
			std::basic_ostream<token> &out;
		public:
			tokenizer(std::istream &in, std::basic_ostream<token> &out)
				: in(in), out(out) {}
		};
	}
}

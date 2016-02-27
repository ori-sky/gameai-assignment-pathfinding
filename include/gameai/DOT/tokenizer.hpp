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
			tokenizer(decltype(in) &in, decltype(out) &out) : in(in), out(out) {}
		};
	}
}

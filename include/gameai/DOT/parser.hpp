#pragma once
#include <iostream>
#include "token.hpp"

namespace gameai {
	namespace DOT {
		class parser {
		private:
			std::basic_istream<token> &in;
		public:
			parser(std::basic_istream<token> &in) : in(in) {}
		};
	}
}

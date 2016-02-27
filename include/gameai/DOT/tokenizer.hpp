#pragma once
#include <iostream>
#include "token.hpp"

namespace gameai {
	namespace DOT {
		template<typename T> class tokenizer : public std::basic_istream<token> {
		private:
			T in;
		public:
			template<typename ...Ts> tokenizer(Ts &&...args) : in(std::forward<Ts>(args)...) {}
		};
	}
}

#pragma once
#include <streambuf>
#include "token.hpp"

namespace gameai {
	namespace DOT {
		template<typename T> class tokenbuf : public std::basic_streambuf<token> {
			static_assert(std::is_base_of<std::istream, T>::value, "tokenbuf requires template argument of base class std::istream");
		private:
			T in;
			token tok;
		public:
			template<typename ...Ts> tokenbuf(Ts &&...args) : in(std::forward<Ts>(args)...) {
				token *end = &tok + sizeof(token);
				setg(&tok, end, end);
			}

			int_type underflow() {
				if(gptr() >= egptr()) {
					char c = 0;
					in >> c;
					switch(c) {
					case '{':
						tok = token::curly_brace_open;
						break;
					case '}':
						tok = token::curly_brace_close;
						break;
					case '[':
						tok = token::square_bracket_open;
						break;
					case ']':
						tok = token::square_bracket_close;
						break;
					case 'g': // temporary test
						tok = token::identifier;
						break;
					default:
						return traits_type::eof();
					}
					setg(&tok, &tok, &tok + sizeof(token));
				}
				return traits_type::to_int_type(*gptr());
			}
		};
		template<typename T> class tokenizer : public std::basic_istream<token> {
		private:
			DOT::tokenbuf<T> buf;
		public:
			template<typename ...Ts> tokenizer(Ts &&...args) :
			  buf(std::forward<Ts>(args)...),
			  std::basic_istream<token>(&buf) {}
		};
	}
}

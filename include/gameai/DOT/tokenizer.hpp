#pragma once
#include <iostream>
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
				setg(&tok, &tok, &tok + 1);
			}

			int_type underflow() {
				if(gptr() >= egptr()) {
					char c;
					in >> c;
					std::cerr << "c=" << c << ' ';
					switch(c) {
					case ' ':
					case '\r':
					case '\n':
					case ';':
						return underflow();
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
					case '=':
						tok = token::equals;
						break;
					case ',':
						tok = token::comma;
						break;
					default:
						if((c >= 'A' && c <= 'Z') ||
						   (c >= 'a' && c <= 'z') ||
						   (c >= '0' && c <= '9') ||
						    c == '.' ||
						    c == '"') {
							tok = token::identifier;
							break;
						} else {
							return traits_type::eof();
						}
					}
					setg(&tok, &tok, &tok + 1);
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

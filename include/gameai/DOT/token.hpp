#pragma once

namespace gameai {
	namespace DOT {
		enum class token {
			curly_brace_open,
			curly_brace_close,
			square_bracket_open,
			square_bracket_close,
			equals,
			comma,
			undirected_relation,
			identifier
		};
		std::ostream & operator<<(std::ostream &s, const token &tok) {
			switch(tok) {
			case token::curly_brace_open:
				return s << "<token::curly_brace_open>";
			case token::curly_brace_close:
				return s << "<token::curly_brace_close>";
			case token::square_bracket_open:
				return s << "<token::square_bracket_open>";
			case token::square_bracket_close:
				return s << "<token::square_bracket_open>";
			case token::equals:
				return s << "<token::equals>";
			case token::comma:
				return s << "<token::comma>";
			case token::identifier:
				return s << "<token::identifier>";
			default:
				return s << "<token::undefined>";
			}
		}
	}
}

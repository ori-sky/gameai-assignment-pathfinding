#pragma once

namespace gameai {
	template<typename T> struct inverse_comparator {
		bool operator()(const T &lhs, const T &rhs) const {
			return lhs > rhs;
		}
	};
}

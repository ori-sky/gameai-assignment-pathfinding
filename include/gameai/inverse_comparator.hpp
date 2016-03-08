#pragma once

#include <boost/shared_ptr.hpp>

namespace gameai {
	template<typename T> struct inverse_comparator {
		bool operator()(const T &lhs, const T &rhs) const {
			return lhs > rhs;
		}
	};

	template<typename T> struct inverse_shared_ptr_comparator {
		bool operator()(const boost::shared_ptr<T> lhs, const boost::shared_ptr<T> rhs) const {
			return *lhs > *rhs;
		}
	};
}

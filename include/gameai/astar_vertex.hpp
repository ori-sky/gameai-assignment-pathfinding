#pragma once

namespace gameai {
	template<typename T> struct astar_vertex {
		float base_cost;
		float end_cost;
		unsigned int name;
		T vertex;

		float total_cost() const {
			return base_cost + end_cost;
		}

		friend bool operator==(const astar_vertex &lhs, const astar_vertex &rhs) {
			return lhs.name == rhs.name;
		}

		friend bool operator<(const astar_vertex &lhs, const astar_vertex &rhs) {
			return lhs.total_cost() < rhs.total_cost();
		}

		friend bool operator>(const astar_vertex &lhs, const astar_vertex &rhs) {
			return lhs.total_cost() > rhs.total_cost();
		}
	};
	template<typename T> std::size_t hash_value(const astar_vertex<T> &v) {
		boost::hash<float> hasher;
		return hasher(v.total_cost());
	}
}

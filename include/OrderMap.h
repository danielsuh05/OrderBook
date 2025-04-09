//
// Created by Daniel Suh on 4/9/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ORDERMAP_H_
#define SIMPLEORDERBOOK_INCLUDE_ORDERMAP_H_

#include <cstdint>
#include <vector>

template <typename T>
class OrderMap {
 public:
	inline void ensureSize(size_t oid) {
		if(oid >= size_) {
			data_.resize(oid + 1);
		}
	}

	inline void reserve(size_t n) {
		data_.reserve(n);
	}

	inline T& get(size_t oid) {
		return data_[oid];
	}
 private:
	std::vector<T> data_;
	size_t size_;
};

#endif //SIMPLEORDERBOOK_INCLUDE_ORDERMAP_H_

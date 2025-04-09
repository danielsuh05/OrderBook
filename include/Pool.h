//
// Created by Daniel Suh on 4/9/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_POOL_H_
#define SIMPLEORDERBOOK_INCLUDE_POOL_H_

#include <cstdint>
#include <vector>

template <typename T>
class Pool {
 public:
	inline Pool(size_t size) {
		data_.reserve(size);
	}

	inline T& get(size_t idx) {
		return data_[idx];
	}

 private:
	std::vector<T> data_;
	std::vector<uint32_t> freeList_;
	size_t size_;
};

#endif //SIMPLEORDERBOOK_INCLUDE_POOL_H_

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
    freeList_.reserve(size);
    size_ = size;
  }

  inline T& get(size_t idx) { return data_[idx]; }

  inline uint32_t alloc() {
    if (freeList_.empty()) {
      auto ret = data_.size();
      data_.push_back(T());
      return ret;
    } else {
      uint32_t freeIdx = freeList_.back();
      freeList_.pop_back();
      return freeIdx;
    }
  }

  inline void free(uint32_t idx) { freeList_.push_back(idx); }

 private:
  std::vector<T> data_;
  std::vector<uint32_t> freeList_;
  size_t size_;
};

#endif  // SIMPLEORDERBOOK_INCLUDE_POOL_H_

//
// Created by Daniel Suh on 4/9/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_LEVELPTR_H_
#define SIMPLEORDERBOOK_INCLUDE_LEVELPTR_H_

#include <cstdint>

struct LevelPtr {
  int32_t price_;
  uint32_t idx_;

  LevelPtr(int32_t price, uint32_t idx) : price_{price}, idx_{idx} {}
};

#endif  // SIMPLEORDERBOOK_INCLUDE_LEVELPTR_H_

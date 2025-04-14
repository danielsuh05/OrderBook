//
// Created by Daniel Suh on 4/9/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_LEVEL_H_
#define SIMPLEORDERBOOK_INCLUDE_LEVEL_H_

#include <cstdint>

/**
 * @brief The level on the order book. Contains the quantity for each respective
 * price.
 */
struct Level {
  // price_ < 0 if ask, price > 0 if bid
  int32_t price_;
  uint32_t qty_;

  Level(int32_t price, uint32_t qty) : price_{price}, qty_{qty} {}

  Level() : Level(0, 0) {}
};

#endif  // SIMPLEORDERBOOK_INCLUDE_LEVEL_H_

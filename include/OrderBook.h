//
// Created by Daniel Suh on 4/9/2025
//

#ifndef SIMPLEORDERBOOK_ORDER_BOOK_H
#define SIMPLEORDERBOOK_ORDER_BOOK_H

#include <cstdint>
#include <iostream>
#include <system_error>
#include <vector>

#include "Level.h"
#include "LevelPtr.h"
#include "Order.h"
#include "Pool.h"
#include "Side.h"

class OrderBook {
 public:
  OrderBook();

  void preallocate(size_t levelsPerSide) {
    asks.reserve(levelsPerSide);
    bids.reserve(levelsPerSide);
  }

  void addOrder(Order& order, int32_t price, uint32_t qty, Side side,
                Pool<Level>& pool);

  void deleteOrder(Order& order, Pool<Level>& pool);

  // Reduces order by qty
  void cancelOrder(Order& order, uint32_t qty, Pool<Level>& pool);

  void print(Pool<Level>& pool) const;

  uint64_t numIterations{0};
  uint64_t times{0};

 private:
  std::vector<LevelPtr> asks;
  std::vector<LevelPtr> bids;
};

#endif
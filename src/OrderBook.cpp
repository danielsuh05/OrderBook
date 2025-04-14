//
// Created by Daniel Suh on 4/9/25.
//

#include "OrderBook.h"

#include "Side.h"

OrderBook::OrderBook() {
  //	asks.reserve(10000);
  //	bids.reserve(10000);
}

/**
 * @brief Adds an order for a specific stock locale. Will update quantity if
 * price exists, else creates new price
 * @param order The order to add to the book
 * @param price The associated price with the order
 * @param qty The quantity of the order
 * @param side Whether the order is on the bid or ask side
 * @param pool The allocating pool
 */
void OrderBook::addOrder(Order& order, int32_t price, uint32_t qty, Side side,
                         Pool<Level>& pool) {
  std::vector<LevelPtr>& levels = (side == Side::Ask ? asks : bids);
  price *= (side == Side::Ask ? -1 : 1);

  // Binary search for price
  auto it = std::lower_bound(
      levels.begin(), levels.end(), price,
      [](const LevelPtr& level, int32_t p) { return level.price_ < p; });

  bool found = (it != levels.end() && it->price_ == price);

  if (found) {
    order.levelIdx_ = it->idx_;
  } else {
    // Allocate new Level on the pool
    uint32_t allocatedLevelIdx = pool.alloc();
    order.levelIdx_ = allocatedLevelIdx;
    pool.get(allocatedLevelIdx).price_ = price;
    pool.get(allocatedLevelIdx).qty_ = 0;

    const LevelPtr ptr{price, allocatedLevelIdx};
    levels.insert(it, ptr);
  }

  pool.get(order.levelIdx_).qty_ += qty;
}

/**
 * @brief Deletes an order for a specific stock locale.
 * @param order The order to delete from the book
 * @param pool The allocating pool
 */
void OrderBook::deleteOrder(Order& order, Pool<Level>& pool) {
  pool.get(order.levelIdx_).qty_ -= order.qty_;
  if (pool.get(order.levelIdx_).qty_ == 0) {
    int32_t price = pool.get(order.levelIdx_).price_;
    std::vector<LevelPtr>& levels = (price > 0 ? bids : asks);

    auto it = levels.end();
    while (it-- != levels.end()) {
      if (it->price_ == price) {
        levels.erase(it);
        break;
      }
    }
    pool.free(order.levelIdx_);
  }
}

/**
 * @brief Cancel an order for a specific stock locale. Cancel means to reduce
 * the order.
 * @param order The order to delete from the book
 * @param qty The amount to reduce the order by
 * @param pool The allocating pool
 */
void OrderBook::cancelOrder(Order& order, uint32_t qty, Pool<Level>& pool) {
  pool.get(order.levelIdx_).qty_ -= qty;
  order.qty_ -= qty;
}

/**
 * @brief Prints the specific order book.
 * @param pool The allocating pool
 */
void OrderBook::print(Pool<Level>& pool) const {
  std::cout << "  Bids (Buy side):" << std::endl;
  if (bids.empty()) {
    std::cout << "    (Empty)" << std::endl;
  }
  for (const auto& levelPtr : bids) {
    std::cout << "    " << pool.get(levelPtr.idx_).price_ << "\t\t"
              << pool.get(levelPtr.idx_).qty_ << std::endl;
  }

  std::cout << "  Asks (Sell side):" << std::endl;
  if (asks.empty()) {
    std::cout << "    (Empty)" << std::endl;
  }
  for (const auto& levelPtr : asks) {
    std::cout << "    " << pool.get(levelPtr.idx_).price_ << "\t\t"
              << pool.get(levelPtr.idx_).qty_ << std::endl;
  }
}
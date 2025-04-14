//
// Created by Daniel Suh on 4/9/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ORDERBOOKMANAGER_H_
#define SIMPLEORDERBOOK_INCLUDE_ORDERBOOKMANAGER_H_

#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "Level.h"
#include "Order.h"
#include "OrderBook.h"
#include "OrderMap.h"
#include "Pool.h"
#include "Side.h"

/**
 * The overarching obect that maintains all of the specific order books for each
 * stock ticker.
 */
class OrderBookManager {
 public:
  OrderBookManager();

  void addOrder(uint32_t oid, int32_t price, uint32_t qty, Side side,
                uint32_t stockLocate);

  void deleteOrder(uint32_t oid);

  void cancelOrder(uint32_t oid, uint32_t qty);

  void executeOrder(uint32_t oid, uint32_t qty);

  void replaceOrder(uint32_t oldOid, uint32_t newOid, uint32_t newQuantity,
                    int32_t newPrice);

 private:
  std::vector<OrderBook> books;
  Pool<Level> pool;
  OrderMap<Order> oids;
};

#endif  // SIMPLEORDERBOOK_INCLUDE_ORDERBOOKMANAGER_H_

//
// Created by Daniel Suh on 4/9/25.
//

#include "OrderBookManager.h"

#include "Side.h"

/**
 * @brief Creates an OrderBookManager object
 */
OrderBookManager::OrderBookManager()
    : books(1 << 14), pool(Constants::kPoolSize) {
  oids.reserve(Constants::kMaxOID * 2);

  constexpr size_t LEVELS_PER_SIDE = 1 << 3;
  for (auto& book : books) {
    book.preallocate(LEVELS_PER_SIDE);
  }
}

/**
 * @brief Adds an order to the order book specified by the stockLocate.
 * @param oid The ID of the order to add to the book
 * @param price The associated price with the order
 * @param qty The quantity of the order
 * @param side Whether the order is on the bid or ask side
 * @param stockLocate The locate of the stock's order book you want the order to
 * be added to
 */
void OrderBookManager::addOrder(uint32_t oid, int32_t price, uint32_t qty,
                                Side side, uint32_t stockLocate) {
  oids.ensureSize(oid);
  Order& curOrder = oids.get(oid);
  curOrder.qty_ = qty;
  curOrder.stockLocate_ = stockLocate;

  books[stockLocate].addOrder(curOrder, price, qty, side, pool);
}

/**
 * @brief Deletes an order to the order book specified by an order ID
 * @param oid The ID of the order to delete from the book
 */
void OrderBookManager::deleteOrder(uint32_t oid) {
  Order& curOrder = oids.get(oid);

  books[curOrder.stockLocate_].deleteOrder(curOrder, pool);
}

/**
 * @brief Cancels an order to the order book specified by an order ID
 * @param oid The ID of the order to delete from the book
 * @param qty The quantity to reduce the order by
 */
void OrderBookManager::cancelOrder(uint32_t oid, uint32_t qty) {
  Order& curOrder = oids.get(oid);
  books[curOrder.stockLocate_].cancelOrder(curOrder, qty, pool);
}

/**
 * @brief Executes an order to the order book specified by an order ID
 * @param oid The ID of the order to execute
 * @param qty The quantity of the execution
 */
void OrderBookManager::executeOrder(uint32_t oid, uint32_t qty) {
  Order& curOrder = oids.get(oid);

  if (qty == pool.get(curOrder.levelIdx_).qty_) {
    books[curOrder.stockLocate_].deleteOrder(curOrder, pool);
  } else {
    books[curOrder.stockLocate_].cancelOrder(curOrder, qty, pool);
  }
}

/**
 * @brief Replaces an order specified by some order ID with a new order
 * @param oldOid The ID of the order to delete
 * @param newOid The ID of the order to add
 * @param newQuantity The quantity associated with the priceof the new order
 * @param newPrice The price of the new order
 */
void OrderBookManager::replaceOrder(uint32_t oldOid, uint32_t newOid,
                                    uint32_t newQuantity, int32_t newPrice) {
  Order& curOrder = oids.get(oldOid);
  Side side = (pool.get(curOrder.levelIdx_).price_ > 0 ? Side::Bid : Side::Ask);
  uint32_t stockLocate = curOrder.stockLocate_;

  books[curOrder.stockLocate_].deleteOrder(curOrder, pool);

  addOrder(newOid, newPrice, newQuantity, side, stockLocate);
}

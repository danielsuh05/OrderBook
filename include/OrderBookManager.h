//
// Created by Daniel Suh on 4/9/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ORDERBOOKMANAGER_H_
#define SIMPLEORDERBOOK_INCLUDE_ORDERBOOKMANAGER_H_

#include "OrderBook.h"
#include "Pool.h"
#include "Order.h"
#include "Level.h"
#include "Constants.h"
#include "OrderMap.h"

#include <vector>
#include <unordered_map>

class OrderBookManager {
 public:
	OrderBookManager() : books { }, pool { Constants::kPoolSize } {
		oids.reserve(Constants::kMaxOID * 2);
	}

	void addOrder(uint32_t oid, int32_t price, uint32_t qty, uint32_t stockLocate) {
		oids.ensureSize(oid);
		Order& curOrder = oids.get(oid);
		curOrder.qty_ = qty;
		curOrder.stockLocate_ = stockLocate;

		books[stockLocate].addOrder(oid, price, qty);
	}

	void deleteOrder(uint32_t oid) {
		const Order& curOrder = oids.get(oid);
		books[curOrder.stockLocate_].deleteOrder(curOrder);
	}

	void cancelOrder(uint32_t oid, uint32_t qty) {
		const Order& curOrder = oids.get(oid);
		books[curOrder.stockLocate_].cancelOrder(curOrder, qty);
	}

 private:
	std::unordered_map<uint32_t, OrderBook> books;
  Pool<Level> pool;
	OrderMap<Order> oids;
};

#endif //SIMPLEORDERBOOK_INCLUDE_ORDERBOOKMANAGER_H_

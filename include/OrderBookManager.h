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
#include "Side.h"

#include <vector>
#include <unordered_map>

class OrderBookManager {
 public:
	OrderBookManager();

	void addOrder(uint32_t oid, int32_t price, uint32_t qty, Side side, uint32_t stockLocate);

	void deleteOrder(uint32_t oid);

	void cancelOrder(uint32_t oid, uint32_t qty);

	void executeOrder(uint32_t oid, uint32_t qty);

	void replaceOrder(uint32_t oldOid, uint32_t newOid, uint32_t newQuantity, int32_t newPrice);

	void print() {
		std::cout << "Order Book Manager Contents:" << std::endl;

//		for (const auto& kv : books) {
//			uint32_t stockLocate = kv.first;
//			const OrderBook& book = kv.second;
//			std::cout << "Stock Locate " << stockLocate << ":\n";
//			book.print(pool);
//			std::cout << std::endl;
//		}
	}

 private:
	std::vector<OrderBook> books;
	Pool<Level> pool;
	OrderMap<Order> oids;
};

#endif //SIMPLEORDERBOOK_INCLUDE_ORDERBOOKMANAGER_H_

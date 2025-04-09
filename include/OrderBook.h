//
// Created by Daniel Suh on 4/9/2025
//

#ifndef SIMPLEORDERBOOK_ORDER_BOOK_H
#define SIMPLEORDERBOOK_ORDER_BOOK_H

#include <cstdint>
#include <system_error>
#include <vector>

#include "Order.h"
#include "LevelPtr.h"

class OrderBook {
 public:
	void addOrder(uint32_t oid, int32_t price, uint32_t qty) {
	 	throw std::logic_error("Not Implemented");
	}

	void deleteOrder(const Order& order) {
		throw std::logic_error("Not Implemented");
	}

	// Reduces order by qty
	void cancelOrder(const Order& order, uint32_t qty) {
		throw std::logic_error("Not Implemented");
	}

 private:
	std::vector<LevelPtr> asks;
	std::vector<LevelPtr> bids;
};

#endif
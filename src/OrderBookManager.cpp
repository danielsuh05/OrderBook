//
// Created by Daniel Suh on 4/9/25.
//

#include "OrderBookManager.h"
#include "Side.h"

OrderBookManager::OrderBookManager() : books(1 << 14), pool(Constants::kPoolSize) {
    oids.reserve(Constants::kMaxOID * 2);

    constexpr size_t LEVELS_PER_SIDE = 1 << 3;
    for (auto& book : books) {
        book.preallocate(LEVELS_PER_SIDE);
    }
}

void OrderBookManager::addOrder(uint32_t oid, int32_t price, uint32_t qty, Side side, uint32_t stockLocate) {
	oids.ensureSize(oid);
	Order& curOrder = oids.get(oid);
	curOrder.qty_ = qty;
	curOrder.stockLocate_ = stockLocate;

	books[stockLocate].addOrder(curOrder, price, qty, side, pool);
}

void OrderBookManager::deleteOrder(uint32_t oid) {
	Order& curOrder = oids.get(oid);

	books[curOrder.stockLocate_].deleteOrder(curOrder, pool);
}

void OrderBookManager::cancelOrder(uint32_t oid, uint32_t qty) {
	Order& curOrder = oids.get(oid);
	books[curOrder.stockLocate_].cancelOrder(curOrder, qty, pool);
}


void OrderBookManager::executeOrder(uint32_t oid, uint32_t qty) {
	Order& curOrder = oids.get(oid);

	if(qty == pool.get(curOrder.levelIdx_).qty_) {
		books[curOrder.stockLocate_].deleteOrder(curOrder, pool);
	} else {
		books[curOrder.stockLocate_].cancelOrder(curOrder, qty, pool);
	}
}

void OrderBookManager::replaceOrder(uint32_t oldOid, uint32_t newOid, uint32_t newQuantity, int32_t newPrice) {
	Order& curOrder = oids.get(oldOid);
	Side side = (pool.get(curOrder.levelIdx_).price_ > 0 ? Side::Bid : Side::Ask);
	uint32_t stockLocate = curOrder.stockLocate_;

	books[curOrder.stockLocate_].deleteOrder(curOrder, pool);

	addOrder(newOid, newPrice, newQuantity, side, stockLocate);
}


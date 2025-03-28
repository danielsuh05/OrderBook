//
// Created by Daniel Suh on 3/27/25.
//

#ifndef SIMPLEORDERBOOK_ORDER_H
#define SIMPLEORDERBOOK_ORDER_H

#include <cstdint>
#include "Side.h"
#include "OrderType.h"

class Order {
public:
	Order(int32_t price, int32_t quantity, int32_t orderId, OrderType orderType, Side side);

private:
	std::int32_t price_;
	std::int32_t quantity_;
	std::int32_t orderId_;
	OrderType orderType_;
	Side side_;
};


#endif //SIMPLEORDERBOOK_ORDER_H

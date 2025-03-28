//
// Created by Daniel Suh on 3/27/25.
//

#include "Order.h"

Order::Order(int32_t price, int32_t quantity, int32_t orderId, OrderType orderType, Side side) : price_(price),
                                                                                                 quantity_(quantity),
                                                                                                 orderId_(orderId),
                                                                                                 orderType_(orderType),
                                                                                                 side_(side) {}

//
// Created by Daniel Suh on 3/27/25.
//

#ifndef SIMPLEORDERBOOK_ORDER_H
#define SIMPLEORDERBOOK_ORDER_H

#include "Side.h"
#include "ITCHOrderType.h"
#include <cstdint>

struct Order {
  std::int32_t price_;
  std::int32_t quantity_;
  std::int32_t orderId_;
  ITCHOrderType orderType_;
  Side side_;
};

#endif //SIMPLEORDERBOOK_ORDER_H

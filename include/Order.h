//
// Created by Daniel Suh on 4/9/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ORDER_H_
#define SIMPLEORDERBOOK_INCLUDE_ORDER_H_

#include <cstdint>

struct Order {
	uint32_t qty_;
	uint32_t levelIdx_;
	uint32_t stockLocate_;

	Order(uint32_t qty, uint32_t levelIdx, uint32_t bookIdx) : qty_ { qty }, levelIdx_ { levelIdx }, stockLocate_ {bookIdx } {}
	Order() = default;
};

#endif //SIMPLEORDERBOOK_INCLUDE_ORDER_H_

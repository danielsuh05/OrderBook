//
// Created by Daniel Suh on 3/27/25.
//

#ifndef SIMPLEORDERBOOK_ORDERPARSER_H
#define SIMPLEORDERBOOK_ORDERPARSER_H


#include "Order.h"
#include <string_view>

class OrderParser {
public:
	OrderParser() = default;

	// Should not be able to copy an OrderParser object
	OrderParser(const OrderParser &) = delete;

	OrderParser &operator=(const OrderParser &) = delete;

	// Should not be able to move an OrderParser object
	OrderParser(const OrderParser &&) = delete;

	OrderParser &operator=(const OrderParser &&) = delete;

	/**
	 * @brief given a string, parse it into an `Order` object
	 * @param s the string to be parsed
	 */
	Order parseLine(std::string_view);

private:
	OrderType getOrderType(std::string_view);

};


#endif //SIMPLEORDERBOOK_ORDERPARSER_H

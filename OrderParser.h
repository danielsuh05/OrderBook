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

	// Should not be able to copy or move an OrderParser object
	OrderParser(const OrderParser&) = delete;
	OrderParser& operator=(const OrderParser&) = delete;

	OrderParser(const OrderParser&&) = delete;
	OrderParser& operator=(const OrderParser&&) = delete;

	/**
	 * @brief given a string, parse it into an `Order` object
	 * @param s the string to be parsed
	 */
	Order parseLine(char* c);
};


#endif //SIMPLEORDERBOOK_ORDERPARSER_H

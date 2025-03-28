//
// Created by Daniel Suh on 3/27/25.
//

#include "OrderParser.h"

Order OrderParser::parseLine(std::string_view s) {
	return Order(0, 0, 0, OrderType::Market, Side::Buy);
}

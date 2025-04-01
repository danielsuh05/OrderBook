//
// Created by Daniel Suh on 3/27/25.
//

#include "OrderParser.h"
#include <iostream>
#include <string_view>

Order OrderParser::parseLine(std::string_view s) {
	std::vector<std::string_view> tokens;

	std::size_t start = 0;
	while (start < s.size()) {
		// Remove whitespace from improperly formatted order
		while (start < s.size() && s[start] == ' ') start++;

		if (start >= s.size()) break;

		// Find end of token
		size_t end = start;
		while (end < s.size() && s[end] != ' ') end++;

		// O(1) operation because string_view
		std::string_view token = s.substr(start, end - start);
		tokens.push_back(token);

		start = end;
	}

	// Token should be in format: price, quantity, orderType, side
	

	return Order{0, 0, 0, OrderType::Market, Side::Buy};
}

OrderType OrderParser::getOrderType(std::string_view typeString) {
	if (typeString == "limit") {
		return OrderType::Limit;
	} else if (typeString == "market") {
		return OrderType::Market;
	} else {

	}
}

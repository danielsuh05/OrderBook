//
// Created by Daniel Suh on 3/27/25.
//

#ifndef SIMPLEORDERBOOK_ORDERPARSER_H
#define SIMPLEORDERBOOK_ORDERPARSER_H


#include "Order.h"
#include <string_view>

class ITCHParser {
public:
	static ITCHParser& getInstance();
private:
	ITCHParser() = default;

	// Should not be able to copy an ITCHParser object
	ITCHParser(const ITCHParser &) = delete;
	ITCHParser &operator=(const ITCHParser &) = delete;

	// Should not be able to move an ITCHParser object
	ITCHParser(const ITCHParser &&) = delete;
	ITCHParser &operator=(const ITCHParser &&) = delete;
};


#endif //SIMPLEORDERBOOK_ORDERPARSER_H

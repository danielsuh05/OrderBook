//
// Created by Daniel Suh on 3/27/25.
//

#include "ITCHParser.h"
#include <iostream>
#include <string_view>

ITCHParser& ITCHParser::getInstance() {
	static ITCHParser instance{};
	return instance;
}


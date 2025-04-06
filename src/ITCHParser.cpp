//
// Created by Daniel Suh on 4/3/25.
//

#include "ITCHParser.h"
#include "Constants.h"

ITCHParser::ITCHParser(std::size_t bufferSize, int fd)
		: buffer_ { Buffer { bufferSize, fd } } {}

void ITCHParser::parseBuffer() {

}

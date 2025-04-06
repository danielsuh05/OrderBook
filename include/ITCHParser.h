//
// Created by Daniel Suh on 4/3/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_
#define SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_

#include "Buffer.h"

class ITCHParser {
public:
	ITCHParser(std::size_t bufferSize, int fd);

	void parseBuffer();


	// TODO MAKE THIS CLEANER
	Buffer buffer_;
private:
};

#endif //SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_

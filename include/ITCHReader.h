//
// Created by Daniel Suh on 4/3/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ITCHREADER_H_
#define SIMPLEORDERBOOK_INCLUDE_ITCHREADER_H_

#include "Buffer.h"
class ITCHReader {
public:
	ITCHReader(size_t size, int fd) : fd_ { fd }, size_ { size }, buf_ { Buffer{ size }} {}
private:
	int fd_ { -1 };
	size_t size_{};
	Buffer buf_;
};

#endif //SIMPLEORDERBOOK_INCLUDE_ITCHREADER_H_

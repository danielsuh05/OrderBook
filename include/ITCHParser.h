//
// Created by Daniel Suh on 4/3/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_
#define SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_

#include "Buffer.h"
#include "Constants.h"

class ITCHParser {
 public:
  ITCHParser(std::size_t bufferSize, int fd);

  size_t parseBuffer(size_t numBytesReceived);

  Buffer &getBuffer();
  uint64_t numMessages{0};

	char getByte(size_t i) const;

	Buffer buffer_;
	char overflowBuf[Constants::kOverflowBufferSize]{};
	size_t overflowBufSize{0};
};

#endif  // SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_

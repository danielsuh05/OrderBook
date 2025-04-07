//
// Created by Daniel Suh on 4/3/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_BUFFER_H_
#define SIMPLEORDERBOOK_INCLUDE_BUFFER_H_

#include <thread>
#include <cstdlib>
#include <cassert>
#include <cstring>

struct Buffer {
	char* ptr_{};
	const size_t size_{};
	int fd_{};

	explicit Buffer(size_t size, int fd);
	~Buffer();

	bool read(size_t start, char bufferData[]);

	void swapBufferPtrs(Buffer& otherBuf);
};

#endif //SIMPLEORDERBOOK_INCLUDE_BUFFER_H_

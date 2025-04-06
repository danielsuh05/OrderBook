#include "Buffer.h"
#include "ErrorHandler.h"
#include <chrono>
#include <unistd.h>
#include <iostream>

Buffer::Buffer(size_t size, int fd)
		: ptr_(new char[size]),
		  size_(size),
			fd_(fd)
{}

void Buffer::read() {
	ssize_t bytes = ::read(fd_, ptr_, size_);
	if (bytes > 0) {
	} else if (bytes < 0) {
		ErrorHandler::getInstance().postError(Error{"Failure reading the file", std::nullopt, ErrorType::Reading});
	} // if bytes == 0, EOF reached
}


Buffer::~Buffer() {
	delete[] ptr_;
}

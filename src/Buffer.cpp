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

/**
 * @brief Reads in a buffer from the given file descriptor. Will return false if no more data to be read in, returns true if should continue
 */
bool Buffer::read(size_t start, char parserData[]) {
	// TODO: CHECK THIS
	std::memcpy(ptr_, parserData + start, size_ - start);

	ssize_t bytes = ::read(fd_, ptr_ + (size_ - start), start);

	printf("GOT %d BYTES FROM %d\n", bytes, ptr_);

	if (bytes > 0) {
		std::cout << +ptr_[0] << " " << +ptr_[1] << " " << +ptr_[2] << "\n";
		return true;
	} else if (bytes < 0) {
		ErrorHandler::getInstance().postError(Error{"Failure reading the file", std::nullopt, ErrorType::Reading});
		return false;
	} else {
		return false;
	}
}

/**
 * @brief Swaps `otherBuf` pointer with the current object. Used for double buffering setup
 * @param otherBuf The buffer to swap the pointer with
 */
void Buffer::swapBufferPtrs(Buffer& otherBuf) {
	std::swap(this->ptr_, otherBuf.ptr_);
}

Buffer::~Buffer() {
	delete[] ptr_;
}

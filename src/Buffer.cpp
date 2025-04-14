#include "Buffer.h"

#include <unistd.h>

#include <chrono>
#include <iostream>

#include "ErrorHandler.h"

/**
 * @brief Creates a new buffer with a size and a file descriptor which it reads
 * from
 * @param size The size in bytes for how big the buffer should be
 * @param fd The file descriptor number from which to read from
 */
Buffer::Buffer(size_t size, int fd) : size_(size), fd_(fd) {
  ptr_ = new char[size];
}

/**
 * @brief Reads in a buffer from the given file descriptor.
 * @return The number of bytes read in.
 */
size_t Buffer::read() {
  ssize_t bytes = ::read(fd_, ptr_, size_);

  // If failed to read
  if (bytes < 0) {
    ErrorHandler::getInstance().postError(
        Error{"Failure reading the file", std::nullopt, ErrorType::Reading});
  }

  return bytes;
}

/**
 * @brief Swaps `otherBuf` pointer with the current object. Used for double
 * buffering setup
 * @param otherBuf The buffer to swap the pointer with
 */
void Buffer::swapBufferPtrs(Buffer &otherBuf) {
  std::swap(this->ptr_, otherBuf.ptr_);
}

Buffer::~Buffer() { delete[] ptr_; }

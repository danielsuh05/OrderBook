#include "Buffer.h"
#include "ErrorHandler.h"
#include <chrono>
#include <unistd.h>
#include <iostream>

Buffer::Buffer(size_t size)
		: ptr_(new char[size]),
		  size_(size),
		  limit_(0),
		  pos_(0),
		  isRunning_(true)
{
	thread_ = std::thread([this]() {
		while (isRunning_) {
			this->read();
		}
	});
}

void Buffer::read() {
	std::unique_lock<std::mutex> lock(mutex_);
	cv.wait(lock, [this]() { return (limit_ < size_) || !isRunning_; });
	if (!isRunning_) {
		return;
	}
	size_t free = size_ - limit_;
	lock.unlock();
	ssize_t bytes = ::read(fd_, ptr_ + limit_, free);
	lock.lock();
	if (bytes > 0) {
		limit_ += static_cast<size_t>(bytes);
	} else if (bytes == 0) {
		isRunning_ = false;
	} else {
		ErrorHandler::getInstance().postError(Error{"Failure reading the file", std::nullopt, ErrorType::Reading});
		isRunning_ = false;
	}
}

void Buffer::saveAfterPos() {
	std::lock_guard<std::mutex> lock(mutex_);
	if (pos_ < limit_) {
		std::memmove(ptr_, ptr_ + pos_, limit_ - pos_);
		limit_ -= pos_;
		pos_ = 0;
	}
	isRunning_ = true;
	cv.notify_one();
}

Buffer::~Buffer() {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		isRunning_ = false;
		cv.notify_all();
	}
	if (thread_.joinable()) {
		thread_.join();
	}
	delete[] ptr_;
}

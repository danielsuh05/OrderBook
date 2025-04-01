//
// Created by Daniel Suh on 3/28/25.
//

#include "ErrorHandler.h"
#include <iostream>

ErrorHandler &ErrorHandler::getInstance() {
	static ErrorHandler instance{};
	return instance;
}

ErrorHandler::ErrorHandler()
		: isRunning_{false}, thread_{&ErrorHandler::processErrors, this} {}

ErrorHandler::~ErrorHandler() {
	isRunning_ = false;

	// If any cvs are waiting, finish processing them
	cv_.notify_all();

	// Ensure all threads have finished
	if (thread_.joinable()) {
		thread_.join();
	}
}

void ErrorHandler::postError(const Error &error) {
	{
		// RAII lock
		std::scoped_lock<std::mutex> lock{mutex_};
		errors_.push(error);
	}

	// Notify the processErrors thread that there is a new error to be processed
	cv_.notify_one();
}

void ErrorHandler::processErrors() {
	while (isRunning_) {
		std::unique_lock<std::mutex> lock{mutex_};

		// Wait on the lock and prevent spurious wakeup
		cv_.wait(lock, [this] { return !errors_.empty(); });
		while (!errors_.empty()) {
			Error error = errors_.front();
			errors_.pop();
			lock.unlock(); // Done editing internal state

			std::cerr << "[ERROR " << error.errorType_ << "]: " << error.errorMessage_ << "\n";
		}
	}
}
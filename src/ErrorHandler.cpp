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
    : isRunning_{true}, thread_{&ErrorHandler::processErrors, this} {}

ErrorHandler::~ErrorHandler() {
  isRunning_ = false;

  // If any cvs are waiting, finish processing them
  cv_.notify_all();

  // Ensure all threads have finished
  if (thread_.joinable()) {
    thread_.join();
  }
}

/**
 * @brief Post an error to the handler for it to process on a separate thread.
 * @param error The Error object to process.
 */
void ErrorHandler::postError(const Error &error) {
  {
    // RAII lock
    std::scoped_lock<std::mutex> lock{mutex_};
    errors_.push(error);
  }

  // Notify the processErrors thread that there is a new error to be processed
  cv_.notify_one();
}

/**
 * @brief Start to process the errors that are on the queue. Requires that there
 * are errors on the queue or will sleep
 */
void ErrorHandler::processErrors() {
  while (isRunning_) {
    std::queue<Error> localErrors;

    {
      std::scoped_lock<std::mutex> lock(mutex_);
      std::swap(localErrors, errors_);
    }

    while (!localErrors.empty()) {
      Error error = localErrors.front();
      localErrors.pop();
      std::cerr << "[ERROR, " << error.errorType_ << ", "
                << error.orderId_.value_or(-1) << "]: " << error.errorMessage_
                << "\n";
    }
  }
}
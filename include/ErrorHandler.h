//
// Created by Daniel Suh on 3/28/25.
//

#ifndef SIMPLEORDERBOOK_ERRORHANDLER_H
#define SIMPLEORDERBOOK_ERRORHANDLER_H

#include <cstdint>
#include <mutex>
#include <queue>
#include <string_view>
#include <thread>

#include "Error.h"
#include "ITCHOrder.h"

/**
 * @brief Singleton, handles errors on a separate thread
 */
class ErrorHandler {
 public:
  static ErrorHandler &getInstance();

  void postError(const Error &error);
  void processErrors();

  ~ErrorHandler();

  ErrorHandler(const ErrorHandler &) = delete;
  ErrorHandler &operator=(const ErrorHandler &) = delete;

  ErrorHandler(const ErrorHandler &&) = delete;
  ErrorHandler &operator=(const ErrorHandler &&) = delete;

 private:
  ErrorHandler();

  std::condition_variable cv_{};
  std::mutex mutex_{};
  std::atomic<bool> isRunning_{};
  std::queue<Error> errors_{};
  std::thread thread_{};
};

#endif  // SIMPLEORDERBOOK_ERRORHANDLER_H

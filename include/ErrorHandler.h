//
// Created by Daniel Suh on 3/28/25.
//

#ifndef SIMPLEORDERBOOK_ERRORHANDLER_H
#define SIMPLEORDERBOOK_ERRORHANDLER_H

#include <cstdint>
#include <string_view>
#include <mutex>
#include <queue>
#include <thread>
#include "ITCHOrder.h"
#include "Error.h"

/**
 * @brief Singleton, handles errors on a separate thread
 */
class ErrorHandler {
 public:
	static ErrorHandler &getInstance();

	void postError(const Error &error);
	void processErrors();

	~ErrorHandler();

 private:
	ErrorHandler();

	ErrorHandler(const ErrorHandler &) = delete;
	ErrorHandler &operator=(const ErrorHandler &) = delete;

	ErrorHandler(const ErrorHandler &&) = delete;
	ErrorHandler &operator=(const ErrorHandler &&) = delete;

	std::condition_variable cv_{};
	std::mutex mutex_{};
	std::atomic<bool> isRunning_{};
	std::queue<Error> errors_{};
	std::thread thread_{};
};

#endif //SIMPLEORDERBOOK_ERRORHANDLER_H

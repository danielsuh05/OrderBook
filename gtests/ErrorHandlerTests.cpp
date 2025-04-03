#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "ErrorHandler.h"

class CerrRedirect {
	std::streambuf* old_;
	std::ostringstream buffer_;

 public:
	CerrRedirect() {
		old_ = std::cerr.rdbuf(buffer_.rdbuf());
	}

	~CerrRedirect() {
		std::cerr.rdbuf(old_);
	}

	std::string getOutput() const {
		return buffer_.str();
	}
};

/**
 * @brief Ensures that an error message will be printed out
 */
TEST(ErrorHandlerTest, PostsErrorCorrectly) {
	CerrRedirect redirect;

	ErrorHandler& handler = ErrorHandler::getInstance();
	handler.postError({"Critical", 1, ErrorType::Parsing});

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	std::string output = redirect.getOutput();
	EXPECT_NE(output.find("ERROR"), std::string::npos);
}

/**
 * @brief Ensures that multiple error messages will be printed in order
 */
TEST(ErrorHandlerTest, PostsMultipleErrorsCorrectly) {
	CerrRedirect redirect;

	ErrorHandler& handler = ErrorHandler::getInstance();

	constexpr int N = 1'000;
	constexpr std::string s = "s";

	for(int i = 0; i < N; i++) {
		handler.postError({s, i, ErrorType::Parsing});
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	std::string output = redirect.getOutput();

	for(int i = 0; i < N - 1; i++) {
		size_t i1 = output.find(std::to_string(i));
		size_t i2 = output.find(std::to_string(i + 1));
		EXPECT_TRUE(i1 < i2);
	}
}

#include "Buffer.h"
#include "ErrorHandler.h"
#include <gtest/gtest.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <functional>

namespace {
constexpr std::chrono::milliseconds maxWait{1000};
bool waitForCondition(std::function<bool()> condition, std::chrono::milliseconds timeout = maxWait) {
	auto start = std::chrono::steady_clock::now();
	while (!condition() && std::chrono::steady_clock::now() - start < timeout) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return condition();
}
}

class BufferTest : public ::testing::Test {
 protected:
	int pipefd[2];
	void SetUp() override {
		ASSERT_EQ(pipe(pipefd), 0);
	}
	void TearDown() override {
		close(pipefd[0]);
		close(pipefd[1]);
	}
};

TEST_F(BufferTest, ReadOperationUpdatesLimit) {
	const char* testData = "Hello, Buffer!";
	size_t testDataSize = std::strlen(testData);

	Buffer buffer(1024, pipefd[0]);

	ssize_t written = write(pipefd[1], testData, testDataSize);
	ASSERT_EQ(written, static_cast<ssize_t>(testDataSize));

	close(pipefd[1]);

	waitForCondition([&buffer, testDataSize]() { return buffer.limit_ >= testDataSize; });

	EXPECT_GE(buffer.limit_, testDataSize);
	EXPECT_EQ(std::strncmp(buffer.ptr_, testData, testDataSize), 0);
}

TEST_F(BufferTest, SaveAfterPosShiftsData) {
	const char* testData = "DataInBuffer";
	size_t testDataSize = std::strlen(testData);

	Buffer buffer(1024, pipefd[0]);

	ssize_t written = write(pipefd[1], testData, testDataSize);
	ASSERT_EQ(written, static_cast<ssize_t>(testDataSize));

	close(pipefd[1]);

	waitForCondition([&buffer, testDataSize]() { return buffer.limit_ >= testDataSize; });

	size_t consume = 5;

	buffer.pos_ = consume;
	buffer.saveAfterPos();

	EXPECT_EQ(buffer.limit_, testDataSize - consume);
	EXPECT_EQ(buffer.pos_, 0);
	EXPECT_EQ(std::strncmp(buffer.ptr_, testData + consume, testDataSize - consume), 0);
}

TEST_F(BufferTest, EOFHandlingStopsThread) {
	Buffer buffer(1024, pipefd[0]);

	close(pipefd[1]);

	waitForCondition([&buffer]() { return !buffer.isRunning_; });

	EXPECT_FALSE(buffer.isRunning_);
}

TEST_F(BufferTest, ReadMoreThanBufferCapacityAndSaveAfterPos) {
	const size_t bufferSize = 1024;
	std::string testData(2048, 'A');

	testData[1023] = 'B';
	Buffer buffer(bufferSize, pipefd[0]);

	ssize_t written = write(pipefd[1], testData.data(), bufferSize);
	ASSERT_EQ(written, static_cast<ssize_t>(bufferSize));

	waitForCondition([&buffer, bufferSize]() { return buffer.limit_ >= bufferSize; });
	EXPECT_GE(buffer.limit_, bufferSize);

	buffer.pos_ = bufferSize - 5;
	buffer.saveAfterPos();

	EXPECT_EQ(buffer.limit_, 5u);

	std::string expectedFirstPart = testData.substr(bufferSize - 5, 5);
	EXPECT_EQ(std::string(buffer.ptr_, 5), expectedFirstPart);

	std::string testData1(bufferSize, 'B');
	written = write(pipefd[1], testData1.data(), bufferSize);
	ASSERT_EQ(written, static_cast<ssize_t>(bufferSize));

	waitForCondition([&buffer]() { return buffer.limit_ >= bufferSize; });
	std::string firstPart(buffer.ptr_, 5);
	EXPECT_EQ(firstPart, expectedFirstPart);

	std::string secondPart(buffer.ptr_ + 5, 1024 - 5);
	std::string expectedSecondPart = testData1.substr(5, bufferSize - 5);
	EXPECT_EQ(secondPart, expectedSecondPart);

	buffer.pos_ = 1023;
	buffer.saveAfterPos();

	waitForCondition([&buffer]() { return buffer.limit_ == 6u; });
	EXPECT_EQ(buffer.limit_, 6u);

	std::string finalData(buffer.ptr_, 6);
	EXPECT_EQ(finalData, std::string(6, 'B'));

	close(pipefd[1]);
}


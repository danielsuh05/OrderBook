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
	Buffer buffer(1024);
	buffer.setFD(pipefd[0]);
	ssize_t written = write(pipefd[1], testData, testDataSize);
	ASSERT_EQ(written, static_cast<ssize_t>(testDataSize));
	close(pipefd[1]);
	waitForCondition([&buffer, testDataSize]() { return buffer.getLimit() >= testDataSize; });
	EXPECT_GE(buffer.getLimit(), testDataSize);
	EXPECT_EQ(std::strncmp(buffer.getBuffer(), testData, testDataSize), 0);
}

TEST_F(BufferTest, SaveAfterPosShiftsData) {
	const char* testData = "DataInBuffer";
	size_t testDataSize = std::strlen(testData);
	Buffer buffer(1024);
	buffer.setFD(pipefd[0]);
	ssize_t written = write(pipefd[1], testData, testDataSize);
	ASSERT_EQ(written, static_cast<ssize_t>(testDataSize));
	close(pipefd[1]);
	waitForCondition([&buffer, testDataSize]() { return buffer.getLimit() >= testDataSize; });
	size_t consume = 5;
	buffer.setPos(consume);
	buffer.saveAfterPos();
	EXPECT_EQ(buffer.getLimit(), testDataSize - consume);
	EXPECT_EQ(buffer.getPos(), 0);
	EXPECT_EQ(std::strncmp(buffer.getBuffer(), testData + consume, testDataSize - consume), 0);
}

TEST_F(BufferTest, EOFHandlingStopsThread) {
	Buffer buffer(1024);
	buffer.setFD(pipefd[0]);
	close(pipefd[1]);
	waitForCondition([&buffer]() { return !buffer.isRunning(); });
	EXPECT_FALSE(buffer.isRunning());
}

TEST_F(BufferTest, ReadMoreThanBufferCapacityAndSaveAfterPos) {
	const size_t bufferSize = 1024;
	std::string testData(2048, 'A');
	testData[1023] = 'B';
	Buffer buffer(bufferSize);
	buffer.setFD(pipefd[0]);
	ssize_t written = write(pipefd[1], testData.data(), bufferSize);
	ASSERT_EQ(written, static_cast<ssize_t>(bufferSize));
	waitForCondition([&buffer, bufferSize]() { return buffer.getLimit() >= bufferSize; });
	EXPECT_GE(buffer.getLimit(), bufferSize);
	buffer.setPos(bufferSize - 5);
	buffer.saveAfterPos();
	EXPECT_EQ(buffer.getLimit(), 5u);
	std::string expectedFirstPart = testData.substr(bufferSize - 5, 5);
	EXPECT_EQ(std::string(buffer.getBuffer(), 5), expectedFirstPart);
	std::string testData1(bufferSize, 'B');
	written = write(pipefd[1], testData1.data(), bufferSize);
	ASSERT_EQ(written, static_cast<ssize_t>(bufferSize));
	waitForCondition([&buffer]() { return buffer.getLimit() >= 1029u; });
	std::string firstPart(buffer.getBuffer(), 5);
	EXPECT_EQ(firstPart, expectedFirstPart);
	std::string secondPart(buffer.getBuffer() + 5, 1024 - 5);
	std::string expectedSecondPart = testData1.substr(5, bufferSize - 5);
	EXPECT_EQ(secondPart, expectedSecondPart);
	buffer.setPos(1023);
	buffer.saveAfterPos();
	waitForCondition([&buffer]() { return buffer.getLimit() == 6u; });
	EXPECT_EQ(buffer.getLimit(), 6u);
	std::string finalData(buffer.getBuffer(), 6);
	EXPECT_EQ(finalData, std::string(6, 'B'));
	close(pipefd[1]);
}


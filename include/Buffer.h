//
// Created by Daniel Suh on 4/3/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_BUFFER_H_
#define SIMPLEORDERBOOK_INCLUDE_BUFFER_H_

#include <thread>
#include <cstdlib>
#include <cassert>
#include <cstring>

class Buffer {
public:
	explicit Buffer(size_t size);
	~Buffer();

	[[nodiscard]] const char* get(size_t offset) const;

	[[nodiscard]] size_t writableSpace() const;

	void saveAfterPos();

	void read();

	void setFD(int fd);

	size_t getLimit() const { return limit_; }
	size_t getPos() const { return pos_; }
	const char* getBuffer() const { return ptr_; }
	bool isRunning() const { return isRunning_; }
	void setPos(size_t pos) { pos_ = pos; }

private:
	char* ptr_;
	const size_t size_;
	size_t limit_;
	size_t pos_;
	int fd_ = -1;

	std::condition_variable cv{};
	std::mutex mutex_{};
	std::atomic<bool> isRunning_{};
	std::thread thread_{};
};

#endif //SIMPLEORDERBOOK_INCLUDE_BUFFER_H_

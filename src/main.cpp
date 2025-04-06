#include "Buffer.h"
#include "ITCHParser.h"
#include "Constants.h"
#include <thread>
#include <semaphore>

int main() {
	auto buffer = Buffer(Constants::kBufferSize, Constants::kFD);
	auto parser = ITCHParser(Constants::kBufferSize, Constants::kFD);

	std::binary_semaphore signal_to_process{0};
	std::binary_semaphore signal_to_generate{1};

	const int numIterations = 100;

	auto readBuffer = [&]() {
		for(int i = 0; i < numIterations; i++) {
			buffer.read();

			signal_to_generate.acquire();

			std::swap(buffer.ptr_, parser.buffer_.ptr_);

			signal_to_process.release();
		}
	};

	auto parseBuffer = [&]() {
		for(int i = 0; i < numIterations; i++) {
			signal_to_process.acquire();

			parser.parseBuffer();

			signal_to_generate.release();
		}
	};

	std::thread generatorThread { readBuffer };
	std::thread parserThread { parseBuffer };

	generatorThread.join();
	parserThread.join();

	return 0;
}

#include "Buffer.h"
#include "ITCHParser.h"
#include "Constants.h"
#include <thread>
#include <semaphore>
#include <iostream>

int main() {
	auto buffer = Buffer(Constants::kBufferSize, Constants::kFD);
	auto parser = ITCHParser(Constants::kBufferSize, Constants::kFD);

	std::binary_semaphore signal_to_process{0};
	std::binary_semaphore signal_to_generate{1};

	printf("Getter buffer %p\n", buffer.ptr_);
	printf("Parser buffer %p\n", parser.getBuffer().ptr_);

	std::atomic<bool> continueReading = true;
	std::atomic<size_t> offset = Constants::kBufferSize;


	while(continueReading) {
		continueReading = buffer.read(offset, parser.getBuffer().ptr_);

		buffer.swapBufferPtrs(parser.getBuffer());
		std::cout << "SWAPPING BUFFERS\n";

		offset = parser.parseBuffer();
		std::cout << "OFFSET: " << offset << "\n";

//		signal_to_process.release();
	}

	return 0;


	auto readBuffer = [&]() {
		while(continueReading) {
//			continueReading = buffer.read(offset);
//
//			signal_to_generate.acquire();
//
//			buffer.swapBufferPtrs(parser.getBuffer());
//			std::cout << "SWAPPING BUFFERS\n";
//
//			signal_to_process.release();
		}
	};

	auto parseBuffer = [&]() {
		while(continueReading) {
			signal_to_process.acquire();

			offset = parser.parseBuffer();
			std::cout << "OFFSET: " << offset << "\n";

			signal_to_generate.release();
		}
	};

	std::thread generatorThread { readBuffer };
	std::thread parserThread { parseBuffer };

	generatorThread.join();
	parserThread.join();

	return 0;
}

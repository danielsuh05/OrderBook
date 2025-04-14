#include <atomic>
#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>

#include "Buffer.h"
#include "Constants.h"
#include "ITCHParser.h"
#include "OrderBook.h"

int main() {
  auto startTime = std::chrono::high_resolution_clock::now();

  auto buffer = Buffer(Constants::kBufferSize, Constants::kFD);
  auto parser = ITCHParser(Constants::kBufferSize, Constants::kFD);

  std::binary_semaphore signal_to_process{0};
  std::binary_semaphore signal_to_generate{1};

  std::atomic<size_t> numBytesReceived{Constants::kBufferSize};
  std::atomic<size_t> overflowBufSize{0};
  std::atomic<bool> readingDone{false};

  auto readBuffer = [&]() {
    while (true) {
      signal_to_generate.acquire();
      size_t bytesRead = buffer.read();
      numBytesReceived.store(bytesRead);

      // Swap the two buffers so the parser can parse it
      buffer.swapBufferPtrs(parser.getBuffer());

      signal_to_process.release();

      // If parsed the last buffer, notify the parser
      if (bytesRead < Constants::kBufferSize) {
        readingDone.store(true);
        break;
      }
    }
  };

  auto parseBuffer = [&]() {
    while (true) {
      signal_to_process.acquire();

      size_t currentNumBytes = numBytesReceived.load();
      bool isReadingDone = readingDone.load();

      size_t newOverflowSize = parser.parseBuffer(currentNumBytes);
      overflowBufSize.store(newOverflowSize);

      // If parsed last buffer, then exit out
      if (isReadingDone && currentNumBytes < Constants::kBufferSize) {
        break;
      }

      signal_to_generate.release();
    }
  };

  std::thread generatorThread{readBuffer};
  std::thread parserThread{parseBuffer};

  generatorThread.join();
  parserThread.join();

  auto endTime = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
  std::cout << "Parsed " << parser.numMessages << " messages in "
            << elapsed.count() / 1000.0 << " s." << std::endl;

  return 0;
}
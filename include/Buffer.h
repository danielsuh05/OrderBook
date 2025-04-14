//
// Created by Daniel Suh on 4/3/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_BUFFER_H_
#define SIMPLEORDERBOOK_INCLUDE_BUFFER_H_

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <thread>

/**
 * @brief A buffer that holds some number of bytes. Has the ability to read from
 * a file descriptor.
 */
struct Buffer {
  char *ptr_{};
  const size_t size_{};
  int fd_{};

  explicit Buffer(size_t size, int fd);
  ~Buffer();

  size_t read();

  void swapBufferPtrs(Buffer &otherBuf);
};

#endif  // SIMPLEORDERBOOK_INCLUDE_BUFFER_H_

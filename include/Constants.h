//
// Created by Daniel Suh on 4/6/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_CONSTANTS_H_
#define SIMPLEORDERBOOK_INCLUDE_CONSTANTS_H_

#include <cstdlib>

namespace Constants {
inline constexpr size_t kBufferSize = 1024 * 1000;    // in # of bytes
inline constexpr size_t kFD = 0;                   // stdin
inline constexpr size_t kOverflowBufferSize = 51;  // in # of bytes
};  // namespace Constants

#endif  // SIMPLEORDERBOOK_INCLUDE_CONSTANTS_H_

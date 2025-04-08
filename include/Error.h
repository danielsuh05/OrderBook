//
// Created by Daniel Suh on 3/28/25.
//

#ifndef SIMPLEORDERBOOK_ERROR_H
#define SIMPLEORDERBOOK_ERROR_H

#include <string>
#include <string_view>

#include "ErrorType.h"

struct Error {
  std::string errorMessage_;
  std::optional<int32_t> orderId_;
  ErrorType errorType_;
};

#endif  // SIMPLEORDERBOOK_ERROR_H

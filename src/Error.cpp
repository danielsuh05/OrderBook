//
// Created by Daniel Suh on 3/28/25.
//

#include "Error.h"

Error::Error(std::string_view errorMessage, std::optional<int32_t> orderId, ErrorType errorType)
				: errorMessage_{errorMessage}, orderId_{orderId}, errorType_{errorType} {}

Error::Error(std::string_view errorMessage, ErrorType errorType)
				: Error(errorMessage, std::nullopt, errorType) {}
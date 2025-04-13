//
// Created by Daniel Suh on 4/3/25.
//

#include "ITCHParser.h"

#include "Constants.h"
#include "ErrorHandler.h"
#include "ITCHMessageType.h"
#include "OrderBook.h"

/**
 * @brief Creates an ITCHParser instance. Requires a buffer to read from,
 * parameters for this passed in
 * @param bufferSize The size of the internal buffer
 * @param fd The file descriptor number of the internal buffer
 */
ITCHParser::ITCHParser(std::size_t bufferSize, int fd)
    : buffer_{Buffer{bufferSize, fd}}, book_{} {}

// Will get a byte depending on if it is in the previous buffer or the
// current buffer
char ITCHParser::getByte(size_t i) const {
  if (i < overflowBufSize_) {
    return overflowBuf_[i];
  } else {
    return buffer_.ptr_[i - overflowBufSize_];
  }
};

/**
 * @brief Parses the internal buffer of the ITCHParser object.
 * @param numBytesReceived The number of bytes to parse
 */
size_t ITCHParser::parseBuffer(size_t numBytesReceived) {
  const size_t totalSize = overflowBufSize_ + numBytesReceived;

  size_t pos = 0;

  // Always need to read in 2 bytes for the length of the next message
  while (pos + 2 < totalSize) {
    const char code = getByte(pos + 2);
    const auto type = static_cast<ITCHMessageType>(code);
    const size_t messageLen = getMessageLength(type);

#ifdef DEBUG
    const auto high = static_cast<uint16_t>(static_cast<uint8_t>(getByte(pos)));
    const auto low =
        static_cast<uint16_t>(static_cast<uint8_t>(getByte(pos + 1)));
    const size_t comp = (high << 8) | low;

    assert(messageLen == comp);  // sanity check
#endif

    if (pos + 2 + messageLen > totalSize) {
      break;
    }
    numMessages++;

    // Do casework based on the type of the message. We only care about ones
    // that affect the order book
    switch (type) {
      case ITCHMessageType::ADD_ORDER:
      case ITCHMessageType::ADD_ORDER_MPID: {
        auto order =
            ITCHOrder<ITCHMessageType::ADD_ORDER>::parse(*this, pos + 2);

				book_.addOrder(order.id_, static_cast<int>(order.price_), order.quantity_, order.side_, order.stockLocate_);
        break;
      }
      case ITCHMessageType::EXECUTE_ORDER:
      case ITCHMessageType::EXECUTE_ORDER_PRICE: {
        auto order =
            ITCHOrder<ITCHMessageType::EXECUTE_ORDER>::parse(*this, pos + 2);

	      book_.executeOrder(order.id_, order.quantity_);
        break;
      }
      case ITCHMessageType::CANCEL_ORDER: {
        auto order =
            ITCHOrder<ITCHMessageType::CANCEL_ORDER>::parse(*this, pos + 2);

				book_.cancelOrder(order.id_, order.numCancelled_);
        break;
      }
      case ITCHMessageType::DELETE_ORDER: {
        auto order =
            ITCHOrder<ITCHMessageType::DELETE_ORDER>::parse(*this, pos + 2);

				book_.deleteOrder(order.id_);
        break;
      }
      case ITCHMessageType::REPLACE_ORDER: {
        auto order =
            ITCHOrder<ITCHMessageType::REPLACE_ORDER>::parse(*this, pos + 2);

	      book_.replaceOrder(order.oldId_, order.newId_, order.quantity_, static_cast<int32_t>(order.price_));
        break;
      }
      case ITCHMessageType::SYSTEM_EVENT:
      case ITCHMessageType::STOCK_DIRECTORY:
      case ITCHMessageType::TRADING_ACTION:
      case ITCHMessageType::REG_SHO:
      case ITCHMessageType::MARKET_PARTICIPANT_POSITION:
      case ITCHMessageType::MWCB_DECLINE:
      case ITCHMessageType::MWCB_BREACH:
      case ITCHMessageType::IPO_QUOTATION:
      case ITCHMessageType::LULD_AUCTION_COLLAR:
      case ITCHMessageType::OPERATION_HALT:
      case ITCHMessageType::TRADE_MESSAGE:
      case ITCHMessageType::CROSS_TRADE_MESSAGE:
      case ITCHMessageType::BROKE_TRADE_MESSAGE:
      case ITCHMessageType::NOII_MESSAGE:
      case ITCHMessageType::RETAIL_INTEREST_MESSAGE:
      case ITCHMessageType::DLCR_MESSAGE:
        break;
      default:
        std::string errorMessage = "Unknown message code ";
        errorMessage += code;
        ErrorHandler::getInstance().postError(
            Error{errorMessage, std::nullopt, ErrorType::Parsing});
        break;
    }

    // 2 for the length + length of message
    pos += 2 + messageLen;
  }

#ifdef DEBUG
  assert(numRemaining <= Constants::kOverflowBufferSize);
#endif

  // Number of remaining bytes that have not been parsed
  const size_t bytesToCopy = totalSize - pos;

  if (bytesToCopy == 0) {
	  overflowBufSize_ = 0;
    return 0;
  }

  if (pos < overflowBufSize_) {
    size_t numAvailableBytes = overflowBufSize_ - pos;
    size_t bytesFromNewBuffer = bytesToCopy - numAvailableBytes;

    // Copy into overflow buffer
    memcpy(overflowBuf_ + numAvailableBytes, buffer_.ptr_, bytesFromNewBuffer);
  } else {
    size_t start = pos - overflowBufSize_;

    // Copy into overflow buffer
    memcpy(overflowBuf_, buffer_.ptr_ + start, bytesToCopy);
  }

	overflowBufSize_ = bytesToCopy;

  return bytesToCopy;
}

/**
 * @brief Gets the underlying buffer. Used for swapping buffers.
 */
Buffer &ITCHParser::getBuffer() { return buffer_; }

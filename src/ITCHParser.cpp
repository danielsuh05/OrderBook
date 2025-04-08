//
// Created by Daniel Suh on 4/3/25.
//

#include "ITCHParser.h"
#include "Constants.h"
#include "ErrorHandler.h"
#include "ITCHMessageType.h"

ITCHParser::ITCHParser(std::size_t bufferSize, int fd)
		: buffer_ { Buffer { bufferSize, fd } } {}

size_t ITCHParser::parseBuffer(char overflowBuf[51], size_t oldOverflowBufSize, size_t numBytesReceived) {
	const size_t totalSize = oldOverflowBufSize + numBytesReceived;

	auto getByte = [&](size_t i) -> char {
		if (i < oldOverflowBufSize) {
			return overflowBuf[i];
		} else {
			assert(i - oldOverflowBufSize < numBytesReceived);
			return buffer_.ptr_[i - oldOverflowBufSize];
		}
	};

	size_t pos = 0;

	while(pos + 2 < totalSize) {
		const char code = getByte(pos + 2);
		const auto type = static_cast<ITCHMessageType>(code);
		const size_t messageLen = getMessageLength(type);

#ifdef DEBUG
		const auto high = static_cast<uint16_t>(static_cast<uint8_t>(getByte(pos)));
		const auto low = static_cast<uint16_t>(static_cast<uint8_t>(getByte(pos + 1)));
		const size_t comp = (high << 8) | low;

		assert(messageLen == comp); // sanity check
#endif

		if (pos + 2 + messageLen > totalSize) {
			break;
		}
		numMessages++;

		switch(type) {
			case ITCHMessageType::ADD_ORDER:
			case ITCHMessageType::ADD_ORDER_MPID:
//				std::cout << "GOT ADD ORDER COMMAND\n";
				break;
			case ITCHMessageType::EXECUTE_ORDER:
			case ITCHMessageType::EXECUTE_ORDER_PRICE:
//				std::cout << "GOT EXECUTE ORDER COMMAND\n";
				break;
			case ITCHMessageType::CANCEL_ORDER:
//				std::cout << "GOT CANCEL ORDER COMMAND\n";
				break;
			case ITCHMessageType::DELETE_ORDER:
//				std::cout << "GOT DELETE ORDER COMMAND\n";
				break;
			case ITCHMessageType::REPLACE_ORDER:
//				std::cout << "GOT REPLACE ORDER COMMAND\n";
				break;
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
				ErrorHandler::getInstance().postError(Error{ errorMessage, std::nullopt, ErrorType::Parsing });
				break;
		}

		pos += 2 + messageLen;
	}

	const size_t numRemaining = totalSize - pos;

	if (numRemaining == 0) {
		return 0;
	}

#ifdef DEBUG
	assert(numRemaining <= Constants::kOverflowBufferSize);
#endif

	size_t bytesToCopy = numRemaining;

	if (pos < oldOverflowBufSize) {
		size_t numAvailableBytes = oldOverflowBufSize - pos;
		size_t bytesFromNewBuffer = bytesToCopy - numAvailableBytes;

		memcpy(overflowBuf + numAvailableBytes, buffer_.ptr_, bytesFromNewBuffer);
	} else {
		size_t start = pos - oldOverflowBufSize;
		memcpy(overflowBuf, buffer_.ptr_ + start, bytesToCopy);
	}

	return bytesToCopy;
}

Buffer& ITCHParser::getBuffer() {
	return buffer_;
}

//
// Created by Daniel Suh on 4/3/25.
//

#include "ITCHParser.h"
#include "Constants.h"
#include "ErrorHandler.h"
#include "ITCHMessageType.h"

ITCHParser::ITCHParser(std::size_t bufferSize, int fd)
		: buffer_ { Buffer { bufferSize, fd } } {}

size_t ITCHParser::parseBuffer() {
	// We have here the abstraction that the buffer will always start at zero because of the work we do before
	size_t pos = 0;
	while(true) {
		if(pos + 2 >= Constants::kBufferSize) {
			break;
		}

		char code = buffer_.ptr_[pos + 2];

		const ITCHMessageType type = static_cast<ITCHMessageType>(code);

		if(pos + 2 + getMessageLength(type) >= Constants::kBufferSize) {
			std::cout << +buffer_.ptr_[pos] << " " << +buffer_.ptr_[pos + 1] << " " << +buffer_.ptr_[pos + 2] << "\n";
			break;
		}

		switch(type) {
			case ITCHMessageType::ADD_ORDER:
			case ITCHMessageType::ADD_ORDER_MPID:
				std::cout << "GOT ADD ORDER COMMAND\n";
				break;
			case ITCHMessageType::EXECUTE_ORDER:
			case ITCHMessageType::EXECUTE_ORDER_PRICE:
				std::cout << "GOT EXECUTE ORDER COMMAND\n";
				break;
			case ITCHMessageType::CANCEL_ORDER:
				std::cout << "GOT CANCEL ORDER COMMAND\n";
				break;
			case ITCHMessageType::DELETE_ORDER:
				std::cout << "GOT DELETE ORDER COMMAND\n";
				break;
			case ITCHMessageType::REPLACE_ORDER:
				std::cout << "GOT REPLACE ORDER COMMAND\n";
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

		pos += 2 + getMessageLength(type);
	}

	return pos;
}

Buffer& ITCHParser::getBuffer() {
	return buffer_;
}

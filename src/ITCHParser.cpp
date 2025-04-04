//
// Created by Daniel Suh on 4/3/25.
//

#include "ITCHParser.h"
#include "ITCHMessageType.h"
#include "ITCHOrder.h"
#include <chrono>
#include <cassert>
#include <iostream>
#include <boost/endian/conversion.hpp>

ITCHParser::ITCHParser(Buffer& buffer)
		: buffer_(buffer), stopParsing_(false)
{
}

ITCHParser::~ITCHParser() {
	stop();
	if (parserThread_.joinable())
		parserThread_.join();
}

void ITCHParser::start() {
	parserThread_ = std::thread(&ITCHParser::parseLoop, this);
}

void ITCHParser::stop() {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		stopParsing_ = true;
	}
	cv_.notify_all();
}

void ITCHParser::parseLoop() {
	constexpr size_t minHeaderSize = 3;
	while (true) {
		{
			std::unique_lock<std::mutex> lock(mutex_);
			if (stopParsing_)
				break;
		}
		size_t available = buffer_.limit_ - buffer_.pos_;
		if (available < minHeaderSize) {
			std::unique_lock<std::mutex> lock(mutex_);
			cv_.wait_for(lock, std::chrono::milliseconds(10));
			continue;
		}
		const char* data = buffer_.ptr_ + buffer_.pos_;
		uint16_t msgLen = boost::endian::big_to_native(*(uint16_t*)data);
		if (available < (msgLen + 2)) {
			std::unique_lock<std::mutex> lock(mutex_);
			cv_.wait_for(lock, std::chrono::milliseconds(10));
			continue;
		}

		ITCHMessageType msgType = static_cast<ITCHMessageType>(data[2]);
		switch (msgType) {
			case ITCHMessageType::SYSTEM_EVENT: {
				auto pkt = ITCHOrder<ITCHMessageType::SYSTEM_EVENT>::parse(buffer_);
				break;
			}
			case ITCHMessageType::STOCK_DIRECTORY: {
				auto pkt = ITCHOrder<ITCHMessageType::STOCK_DIRECTORY>::parse(buffer_);
				break;
			}
			case ITCHMessageType::TRADING_ACTION: {
				auto pkt = ITCHOrder<ITCHMessageType::TRADING_ACTION>::parse(buffer_);
				break;
			}
			case ITCHMessageType::REG_SHO: {
				auto pkt = ITCHOrder<ITCHMessageType::REG_SHO>::parse(buffer_);
				break;
			}
			case ITCHMessageType::MARKET_PARTICIPANT_POSITION: {
				auto pkt = ITCHOrder<ITCHMessageType::MARKET_PARTICIPANT_POSITION>::parse(buffer_);
				break;
			}
			case ITCHMessageType::MWCB_DECLINE: {
				auto pkt = ITCHOrder<ITCHMessageType::MWCB_DECLINE>::parse(buffer_);
				break;
			}
			case ITCHMessageType::MWCB_BREACH: {
				auto pkt = ITCHOrder<ITCHMessageType::MWCB_BREACH>::parse(buffer_);
				break;
			}
			case ITCHMessageType::IPO_QUOTATION: {
				auto pkt = ITCHOrder<ITCHMessageType::IPO_QUOTATION>::parse(buffer_);
				break;
			}
			case ITCHMessageType::LULD_AUCTION_COLLAR: {
				auto pkt = ITCHOrder<ITCHMessageType::LULD_AUCTION_COLLAR>::parse(buffer_);
				break;
			}
			case ITCHMessageType::OPERATION_HALT: {
				auto pkt = ITCHOrder<ITCHMessageType::OPERATION_HALT>::parse(buffer_);
				break;
			}
			case ITCHMessageType::ADD_ORDER: {
				auto pkt = ITCHOrder<ITCHMessageType::ADD_ORDER>::parse(buffer_.ptr_);
				break;
			}
			case ITCHMessageType::ADD_ORDER_MPID: {
				auto pkt = ITCHOrder<ITCHMessageType::ADD_ORDER_MPID>::parse(buffer_);
				break;
			}
			case ITCHMessageType::EXECUTE_ORDER: {
				auto pkt = ITCHOrder<ITCHMessageType::EXECUTE_ORDER>::parse(buffer_.ptr_);
				break;
			}
			case ITCHMessageType::EXECUTE_ORDER_PRICE: {
				auto pkt = ITCHOrder<ITCHMessageType::EXECUTE_ORDER_PRICE>::parse(buffer_);
				break;
			}
			case ITCHMessageType::CANCEL_ORDER: {
				auto pkt = ITCHOrder<ITCHMessageType::CANCEL_ORDER>::parse(buffer_.ptr_);
				break;
			}
			case ITCHMessageType::DELETE_ORDER: {
				auto pkt = ITCHOrder<ITCHMessageType::DELETE_ORDER>::parse(buffer_.ptr_);
				break;
			}
			case ITCHMessageType::REPLACE_ORDER: {
				auto pkt = ITCHOrder<ITCHMessageType::REPLACE_ORDER>::parse(buffer_.ptr_);
				break;
			}
			case ITCHMessageType::TRADE_MESSAGE: {
				auto pkt = ITCHOrder<ITCHMessageType::TRADE_MESSAGE>::parse(buffer_);
				break;
			}
			case ITCHMessageType::CROSS_TRADE_MESSAGE: {
				auto pkt = ITCHOrder<ITCHMessageType::CROSS_TRADE_MESSAGE>::parse(buffer_);
				break;
			}
			case ITCHMessageType::BROKE_TRADE_MESSAGE: {
				auto pkt = ITCHOrder<ITCHMessageType::BROKE_TRADE_MESSAGE>::parse(buffer_);
				break;
			}
			case ITCHMessageType::NOII_MESSAGE: {
				auto pkt = ITCHOrder<ITCHMessageType::NOII_MESSAGE>::parse(buffer_);
				break;
			}
			case ITCHMessageType::RETAIL_INTEREST_MESSAGE: {
				auto pkt = ITCHOrder<ITCHMessageType::RETAIL_INTEREST_MESSAGE>::parse(buffer_);
				break;
			}
			case ITCHMessageType::DLCR_MESSAGE: {
				auto pkt = ITCHOrder<ITCHMessageType::DLCR_MESSAGE>::parse(buffer_);
				break;
			}
			default:
				buffer_.pos_ = buffer_.pos_ + 1;
				break;
		}
		if (buffer_.pos_ > 512) {
			buffer_.saveAfterPos();
		}
	}
}

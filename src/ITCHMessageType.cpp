//
// Created by Daniel Suh on 4/2/25.
//
#include "ITCHMessageType.h"

consteval int getMessageType(ITCHMessageType type) {
	switch(type) {
		case ITCHMessageType::SYSTEM_EVENT:
			return 12;
		case ITCHMessageType::STOCK_DIRECTORY:
			return 39;
		case ITCHMessageType::TRADING_ACTION:
			return 25;
		case ITCHMessageType::REG_SHO:
			return 20;
		case ITCHMessageType::MARKET_PARTICIPANT_POSITION:
			return 26;
		case ITCHMessageType::MWCB_DECLINE:
			return 35;
		case ITCHMessageType::MWCB_BREACH:
			return 12;
		case ITCHMessageType::IPO_QUOTATION:
			return 28;
		case ITCHMessageType::LULD_AUCTION_COLLAR:
			return 35;
		case ITCHMessageType::OPERATION_HALT:
			return 21;
		case ITCHMessageType::ADD_ORDER:
			return 36;
		case ITCHMessageType::ADD_ORDER_MPID:
			return 40;
		case ITCHMessageType::EXECUTE_ORDER:
			return 31;
		case ITCHMessageType::EXECUTE_ORDER_PRICE:
			return 36;
		case ITCHMessageType::CANCEL_ORDER:
			return 23;
		case ITCHMessageType::DELETE_ORDER:
			return 19;
		case ITCHMessageType::REPLACE_ORDER:
			return 35;
		case ITCHMessageType::TRADE_MESSAGE:
			return 44;
		case ITCHMessageType::CROSS_TRADE_MESSAGE:
			return 40;
		case ITCHMessageType::BROKE_TRADE_MESSAGE:
			return 19;
		case ITCHMessageType::NOII_MESSAGE:
			return 50;
		case ITCHMessageType::RETAIL_INTEREST_MESSAGE:
			return 20;
		case ITCHMessageType::DLCR_MESSAGE:
			return 48;
	}
}
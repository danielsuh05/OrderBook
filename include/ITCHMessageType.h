//
// Created by Daniel Suh on 4/2/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ITCHMESSAGETYPE_H_
#define SIMPLEORDERBOOK_INCLUDE_ITCHMESSAGETYPE_H_

enum class ITCHMessageType {
	SYSTEM_EVENT = 'S',
	STOCK_DIRECTORY = 'R',
	TRADING_ACTION = 'H',
	REG_SHO = 'Y',
	MARKET_PARTICIPANT_POSITION = 'L',
	MWCB_DECLINE = 'V',
	MWCB_BREACH = 'W',
	IPO_QUOTATION = 'K',
	LULD_AUCTION_COLLAR = 'J',
	OPERATION_HALT = 'h',
	ADD_ORDER = 'A',
	ADD_ORDER_MPID = 'F',
	EXECUTE_ORDER = 'E',
	EXECUTE_ORDER_PRICE = 'C',
	CANCEL_ORDER = 'X',
	DELETE_ORDER = 'D',
	REPLACE_ORDER = 'U',
	TRADE_MESSAGE = 'P',
	CROSS_TRADE_MESSAGE = 'Q',
	BROKE_TRADE_MESSAGE = 'B',
	NOII_MESSAGE = 'I',
	RETAIL_INTEREST_MESSAGE = 'N',
	DLCR_MESSAGE = 'O'
};

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

#endif //SIMPLEORDERBOOK_INCLUDE_ITCHMESSAGETYPE_H_

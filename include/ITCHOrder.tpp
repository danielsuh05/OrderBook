//
// Created by Daniel Suh on 4/2/25.
//

#include <boost/endian/conversion.hpp>
#include "ITCHParser.h"

// ---------------------- HELPER FUNCTIONS ----------------------
template<typename T>
inline T readBytes(const ITCHParser& p, size_t offset, size_t numBytes) {
	T val;
	char* dest = reinterpret_cast<char*>(&val);
	const size_t endOffset = offset + numBytes;

	if (endOffset <= p.overflowBufSize) {
		memcpy(dest, p.overflowBuf + offset, numBytes);
	} else if (offset >= p.overflowBufSize) {
		memcpy(dest, p.buffer_.ptr_ + (offset - p.overflowBufSize), numBytes);
	} else {
		size_t bytesFromOverflow = p.overflowBufSize - offset;
		size_t bytesFromMain = numBytes - bytesFromOverflow;
		memcpy(dest, p.overflowBuf + offset, bytesFromOverflow);
		memcpy(dest + bytesFromOverflow, p.buffer_.ptr_, bytesFromMain); // Read from start of main buffer
	}
	return val;
}

inline uint64_t read6Bytes(const ITCHParser& p, size_t offset) {
	const size_t numBytes = 6;
	char tempBuf[numBytes];
	const size_t endOffset = offset + numBytes;

	if (endOffset <= p.overflowBufSize) {
		memcpy(tempBuf, p.overflowBuf + offset, numBytes);
	} else if (offset >= p.overflowBufSize) {
		memcpy(tempBuf, p.buffer_.ptr_ + (offset - p.overflowBufSize), numBytes);
	} else {
		size_t bytesFromOverflow = p.overflowBufSize - offset;
		size_t bytesFromMain = numBytes - bytesFromOverflow;
		memcpy(tempBuf, p.overflowBuf + offset, bytesFromOverflow);
		memcpy(tempBuf + bytesFromOverflow, p.buffer_.ptr_, bytesFromMain);
	}

	uint64_t big_endian_value = 0;
	char* dest = reinterpret_cast<char*>(&big_endian_value);

#if BOOST_ENDIAN_NATIVE_BIG
    dest[0] = 0;
    dest[1] = 0;
    memcpy(dest + 2, tempBuf, 6);
#else
	dest[0] = 0;
	dest[1] = 0;
	dest[2] = tempBuf[5];
	dest[3] = tempBuf[4];
	dest[4] = tempBuf[3];
	dest[5] = tempBuf[2];
	dest[6] = tempBuf[1];
	dest[7] = tempBuf[0]; // MSB
#endif

	return boost::endian::big_to_native(big_endian_value);
}

// ---------------------- ADD ORDER WITH OR WITHOUT MPID ----------------------
ITCHOrder<ITCHMessageType::ADD_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, char side, uint32_t volume, uint32_t price)
		: stockLocate_{stockLocate}, timestamp_{timestamp}, id_{id}, side_{charToSide(side)}, quantity_{volume}, price_{price} {}

/**
 * @brief Given a buffer, this function will parse it into the correct type and order
 * @param c The buffer formatted according to ITCH 5.0
 */
ITCHOrder<ITCHMessageType::ADD_ORDER> ITCHOrder<ITCHMessageType::ADD_ORDER>::parse(const ITCHParser& parser, size_t pos) {
	return ITCHOrder<ITCHMessageType::ADD_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(parser, pos + 1, 2)),
			boost::endian::big_to_native(read6Bytes(parser, pos + 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(parser, pos + 11, 8)),
			readBytes<char>(parser, pos + 19, 1),
			boost::endian::big_to_native(readBytes<uint32_t>(parser, pos + 20, 4)),
			boost::endian::big_to_native(readBytes<uint32_t>(parser, pos + 32, 4)),
	};
}

// ---------------------- EXECUTE ORDER ----------------------

ITCHOrder<ITCHMessageType::EXECUTE_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, uint32_t quantity)
	: stockLocate_ { stockLocate }, timestamp_ { timestamp }, id_ { id }, quantity_ { quantity } {}

ITCHOrder<ITCHMessageType::EXECUTE_ORDER> ITCHOrder<ITCHMessageType::EXECUTE_ORDER>::parse(const ITCHParser& parser, size_t pos) {
	return ITCHOrder<ITCHMessageType::EXECUTE_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(parser, pos + 1, 2)),
			boost::endian::big_to_native(read6Bytes(parser, pos + 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(parser, pos + 11, 8)),
			boost::endian::big_to_native(readBytes<uint32_t>(parser, pos + 19, 4))
	};
}

// ---------------------- CANCEL ORDER ----------------------

ITCHOrder<ITCHMessageType::CANCEL_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, uint32_t numCancelled)
		: stockLocate_ { stockLocate }, timestamp_ { timestamp }, id_ { id }, numCancelled_ { numCancelled } {}

ITCHOrder<ITCHMessageType::CANCEL_ORDER> ITCHOrder<ITCHMessageType::CANCEL_ORDER>::parse(const ITCHParser& parser, size_t pos) {
	return ITCHOrder<ITCHMessageType::CANCEL_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(parser, pos + 1, 2)),
			boost::endian::big_to_native(read6Bytes(parser, pos + 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(parser, pos + 11, 8)),
			boost::endian::big_to_native(readBytes<uint32_t>(parser, pos + 19, 4))
	};
}

// ---------------------- CANCEL ORDER ----------------------

ITCHOrder<ITCHMessageType::DELETE_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id)
		: stockLocate_ { stockLocate }, timestamp_ { timestamp }, id_ { id } {}

ITCHOrder<ITCHMessageType::DELETE_ORDER> ITCHOrder<ITCHMessageType::DELETE_ORDER>::parse(const ITCHParser& parser, size_t pos) {
	return ITCHOrder<ITCHMessageType::DELETE_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(parser, pos + 1, 2)),
			boost::endian::big_to_native(read6Bytes(parser, pos + 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(parser, pos + 11, 8)),
	};
}

// ---------------------- REPLACE ORDER ----------------------

ITCHOrder<ITCHMessageType::REPLACE_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t oldId, uint64_t newId, uint32_t quantity, uint32_t price)
		: stockLocate_ { stockLocate }, timestamp_ { timestamp }, oldId_ { oldId }, newId_ { newId }, quantity_ { quantity }, price_ { price } {}

ITCHOrder<ITCHMessageType::REPLACE_ORDER> ITCHOrder<ITCHMessageType::REPLACE_ORDER>::parse(const ITCHParser& parser, size_t pos) {
	return ITCHOrder<ITCHMessageType::REPLACE_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(parser, pos + 1, 2)),
			boost::endian::big_to_native(read6Bytes(parser, pos + 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(parser, pos + 11, 8)),
			boost::endian::big_to_native(readBytes<uint64_t>(parser, pos + 19, 8)),
			boost::endian::big_to_native(readBytes<uint32_t>(parser, pos + 27, 4)),
			boost::endian::big_to_native(readBytes<uint32_t>(parser, pos + 31, 4))
	};
}

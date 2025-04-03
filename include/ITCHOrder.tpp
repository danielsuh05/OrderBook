//
// Created by Daniel Suh on 4/2/25.
//

#include <boost/endian/conversion.hpp>

// ---------------------- HELPER FUNCTIONS ----------------------
/**
 * @brief Will give you `numBytes` bytes from a given buffer at a given offset
 * TODO: PROFILE THIS AGAINST JUST USING POINTER ARITHMETIC
 */
template<typename T>
T readBytes(const char c[], int offset, int numBytes) {
	T val;
	std::memcpy(reinterpret_cast<char*>(&val), c + offset, numBytes);
	return val;
}

/**
 * @brief Need to use a special function for 6 bytes as ITCH is big endian
 */
inline uint64_t read6Bytes(const char c[], int offset) {
	uint64_t val { 0 };
	std::memcpy(reinterpret_cast<char*>(&val) + 2, c + offset, 6);
	return val;
}

// ---------------------- ADD ORDER WITH OR WITHOUT MPID ----------------------

ITCHOrder<ITCHMessageType::ADD_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, char side, uint32_t volume, uint32_t price)
		: stockLocate_{stockLocate}, timestamp_{timestamp}, id_{id}, side_{charToSide(side)}, quantity_{volume}, price_{price} {}

/**
 * @brief Given a buffer, this function will parse it into the correct type and order
 * @param c The buffer formatted according to ITCH 5.0
 */
ITCHOrder<ITCHMessageType::ADD_ORDER> ITCHOrder<ITCHMessageType::ADD_ORDER>::parse(const char c[]) {
	return ITCHOrder<ITCHMessageType::ADD_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(c, 1, 2)),
			boost::endian::big_to_native(read6Bytes(c, 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(c, 11, 8)),
			readBytes<char>(c, 19, 1),
			boost::endian::big_to_native(readBytes<uint32_t>(c, 20, 4)),
			boost::endian::big_to_native(readBytes<uint32_t>(c, 32, 4)),
	};
}

// ---------------------- EXECUTE ORDER ----------------------

ITCHOrder<ITCHMessageType::EXECUTE_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, uint32_t quantity)
	: stockLocate_ { stockLocate }, timestamp_ { timestamp }, id_ { id }, quantity_ { quantity } {}

ITCHOrder<ITCHMessageType::EXECUTE_ORDER> ITCHOrder<ITCHMessageType::EXECUTE_ORDER>::parse(const char c[]) {
	return ITCHOrder<ITCHMessageType::EXECUTE_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(c, 1, 2)),
			boost::endian::big_to_native(read6Bytes(c, 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(c, 11, 8)),
			boost::endian::big_to_native(readBytes<uint32_t>(c, 19, 4))
	};
}

// ---------------------- CANCEL ORDER ----------------------

ITCHOrder<ITCHMessageType::CANCEL_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, uint32_t numCancelled)
		: stockLocate_ { stockLocate }, timestamp_ { timestamp }, id_ { id }, numCancelled_ { numCancelled } {}

ITCHOrder<ITCHMessageType::CANCEL_ORDER> ITCHOrder<ITCHMessageType::CANCEL_ORDER>::parse(const char c[]) {
	return ITCHOrder<ITCHMessageType::CANCEL_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(c, 1, 2)),
			boost::endian::big_to_native(read6Bytes(c, 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(c, 11, 8)),
			boost::endian::big_to_native(readBytes<uint32_t>(c, 19, 4))
	};
}

// ---------------------- CANCEL ORDER ----------------------

ITCHOrder<ITCHMessageType::DELETE_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id)
		: stockLocate_ { stockLocate }, timestamp_ { timestamp }, id_ { id } {}

ITCHOrder<ITCHMessageType::DELETE_ORDER> ITCHOrder<ITCHMessageType::DELETE_ORDER>::parse(const char c[]) {
	return ITCHOrder<ITCHMessageType::DELETE_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(c, 1, 2)),
			boost::endian::big_to_native(read6Bytes(c, 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(c, 11, 8)),
	};
}

// ---------------------- REPLACE ORDER ----------------------

ITCHOrder<ITCHMessageType::REPLACE_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t oldId, uint64_t newId, uint32_t quantity, uint32_t price)
		: stockLocate_ { stockLocate }, timestamp_ { timestamp }, oldId_ { oldId }, newId_ { newId }, quantity_ { quantity }, price_ { price } {}

ITCHOrder<ITCHMessageType::REPLACE_ORDER> ITCHOrder<ITCHMessageType::REPLACE_ORDER>::parse(const char c[]) {
	return ITCHOrder<ITCHMessageType::REPLACE_ORDER>{
			boost::endian::big_to_native(readBytes<uint16_t>(c, 1, 2)),
			boost::endian::big_to_native(read6Bytes(c, 5)),
			boost::endian::big_to_native(readBytes<uint64_t>(c, 11, 8)),
			boost::endian::big_to_native(readBytes<uint64_t>(c, 19, 8)),
			boost::endian::big_to_native(readBytes<uint32_t>(c, 27, 4)),
			boost::endian::big_to_native(readBytes<uint32_t>(c, 31, 4))
	};
}

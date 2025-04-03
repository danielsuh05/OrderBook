//
// Created by Daniel Suh on 4/2/25.
//

#include <boost/endian/conversion.hpp>

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

ITCHOrder<ITCHMessageType::ADD_ORDER>::ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, char side, uint32_t volume, uint32_t price)
		: stockLocate_{stockLocate}, timestamp_{timestamp}, id_{id}, side_{charToSide(side)}, volume_{volume}, price_{price} {}

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

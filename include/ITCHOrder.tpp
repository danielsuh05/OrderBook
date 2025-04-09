//
// Created by Daniel Suh on 4/2/25.
//

#include <boost/endian/conversion.hpp>
#include "ITCHParser.h"

// ---------------------- HELPER FUNCTIONS ----------------------
template<typename T>
inline T readBytes(const ITCHParser& p, size_t offset, size_t numBytes) {
	// Ensure numBytes matches the size of T for safety, although ITCH often uses fixed sizes.
	// static_assert(sizeof(T) == numBytes, "Size mismatch in readBytes"); // Optional check

	T val; // Value to store the result
	char* dest = reinterpret_cast<char*>(&val);
	const size_t endOffset = offset + numBytes;

	// Case 1: The entire read is within the overflow buffer
	if (endOffset <= p.overflowBufSize) {
		memcpy(dest, p.overflowBuf + offset, numBytes);
	}
		// Case 2: The entire read is within the main buffer
	else if (offset >= p.overflowBufSize) {
		memcpy(dest, p.buffer_.ptr_ + (offset - p.overflowBufSize), numBytes);
	}
		// Case 3: The read spans the boundary between overflow and main buffers
	else {
		size_t bytesFromOverflow = p.overflowBufSize - offset;
		size_t bytesFromMain = numBytes - bytesFromOverflow;
		memcpy(dest, p.overflowBuf + offset, bytesFromOverflow);
		memcpy(dest + bytesFromOverflow, p.buffer_.ptr_, bytesFromMain); // Read from start of main buffer
	}
	return val; // Return the value with bytes populated (endianness handled later)
}

/**
 * @brief Optimized: Reads 6 bytes using memcpy, handling buffer boundaries.
 * Returns a uint64_t containing the 6 bytes placed correctly for big-endian interpretation.
 */
inline uint64_t read6Bytes(const ITCHParser& p, size_t offset) {
	const size_t numBytes = 6;
	char tempBuf[numBytes]; // Temporary buffer to hold the 6 bytes
	const size_t endOffset = offset + numBytes;

	// Use memcpy logic similar to readBytes<T> to fill tempBuf
	// Case 1: Fully in overflow
	if (endOffset <= p.overflowBufSize) {
		memcpy(tempBuf, p.overflowBuf + offset, numBytes);
	}
		// Case 2: Fully in main buffer
	else if (offset >= p.overflowBufSize) {
		memcpy(tempBuf, p.buffer_.ptr_ + (offset - p.overflowBufSize), numBytes);
	}
		// Case 3: Spanning the boundary
	else {
		size_t bytesFromOverflow = p.overflowBufSize - offset;
		size_t bytesFromMain = numBytes - bytesFromOverflow;
		memcpy(tempBuf, p.overflowBuf + offset, bytesFromOverflow);
		memcpy(tempBuf + bytesFromOverflow, p.buffer_.ptr_, bytesFromMain);
	}

	// Construct a uint64_t value in memory as if it were big-endian,
	// placing the 6 read bytes into the most significant positions.
	// Then let boost::endian::big_to_native handle the conversion.
	uint64_t big_endian_value = 0; // Initialize to zero
	char* dest = reinterpret_cast<char*>(&big_endian_value);

	// Manually place the bytes assuming dest points to the LSB of big_endian_value
	// on a little-endian machine, or MSB on a big-endian machine.
	// We want to arrange them as if reading a 6-byte big-endian number
	// into the higher-order bytes of a uint64_t.
	// Correct placement for boost::endian::big_to_native:
	// Place the first byte read (MSB of the 6-byte value) at the position
	// corresponding to the MSB of the target uint64_t's representation.
#if BOOST_ENDIAN_NATIVE_BIG
	// Native is Big Endian: Place bytes directly at the start (+2 offset)
    dest[0] = 0;
    dest[1] = 0;
    memcpy(dest + 2, tempBuf, 6);
#else
	// Native is Little Endian: Place bytes reversed into the higher memory addresses
	dest[0] = 0;          // LSB (Padding)
	dest[1] = 0;          // (Padding)
	dest[2] = tempBuf[5]; // LSB of the 6-byte value
	dest[3] = tempBuf[4];
	dest[4] = tempBuf[3];
	dest[5] = tempBuf[2];
	dest[6] = tempBuf[1];
	dest[7] = tempBuf[0]; // MSB of the 6-byte value -> MSB of uint64_t
#endif

	// big_endian_value now holds the bytes arranged as big-endian in memory.
	// big_to_native will convert this to the native value.
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

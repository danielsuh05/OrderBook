//
// Created by Daniel Suh on 4/2/25.
//
#include <gtest/gtest.h>
#include <ITCHOrder.h>
#include <boost/endian/conversion.hpp>

TEST(OrderHandlerTest, AddOrderMessage1) {
	uint8_t buffer[36] = {0};

	buffer[0] = 'A';
	buffer[1] = 0x00; buffer[2] = 0x01;
	buffer[3] = 0x00; buffer[4] = 0x02;
	buffer[5] = 0x00; buffer[6] = 0x2D;
	buffer[7] = 0xC6; buffer[8] = 0xC0;
	buffer[9] = 0x00; buffer[10] = 0x00;
	uint64_t orderRef = boost::endian::native_to_big(12345678ULL); // big endian
	std::memcpy(&buffer[11], &orderRef, 8);
	buffer[19] = 'B';
	buffer[20] = 0x00; buffer[21] = 0x00; buffer[22] = 0x03; buffer[23] = 0xE8;
	std::memcpy(&buffer[24], "AAPL    ", 8);
	uint32_t price = boost::endian::native_to_big(1500000U);
	std::memcpy(&buffer[32], &price, 4);

	auto order = ITCHOrder<ITCHMessageType::ADD_ORDER>::parse(reinterpret_cast<const char*>(buffer));

	ASSERT_EQ(order.stockLocate_, 1);
	std::cout << std::hex << order.timestamp_ << "\n";
	ASSERT_EQ(order.timestamp_, 0x002DC6C00000);
	ASSERT_EQ(order.id_, 12345678);
	ASSERT_EQ(order.side_, Side::Buy);
	ASSERT_EQ(order.price_, 1'500'000); // without conversion
	ASSERT_EQ(order.volume_, 1000);
}


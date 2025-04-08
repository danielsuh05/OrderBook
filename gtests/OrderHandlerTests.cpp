#include <gtest/gtest.h>

#include <boost/endian/conversion.hpp>
#include <cstring>

#include "ITCHOrder.h"

template <typename T>
void writeBigEndian(uint8_t *buffer, T value) {
  T beValue = boost::endian::native_to_big(value);
  std::memcpy(buffer, &beValue, sizeof(T));
}

TEST(OrderHandlerTest, AddOrderMessage) {
  uint8_t buffer[36] = {0};
  buffer[0] = 'A';
  writeBigEndian<uint16_t>(&buffer[1], 1);
  uint64_t timestamp = 0x002DC6C00000ULL;
  uint64_t beTimestamp = boost::endian::native_to_big(timestamp);
  std::memcpy(&buffer[5], reinterpret_cast<uint8_t *>(&beTimestamp) + 2, 6);
  uint64_t orderRef = 12345678ULL;
  writeBigEndian<uint64_t>(&buffer[11], orderRef);
  buffer[19] = 'B';
  writeBigEndian<uint32_t>(&buffer[20], 1000);
  std::memcpy(&buffer[24], "AAPL    ", 8);
  uint32_t price = 1500000U;
  writeBigEndian<uint32_t>(&buffer[32], price);

  auto order = ITCHOrder<ITCHMessageType::ADD_ORDER>::parse(
      reinterpret_cast<const char *>(buffer));
  ASSERT_EQ(order.stockLocate_, 1);
  ASSERT_EQ(order.timestamp_, timestamp & 0x0000FFFFFFFFFFFFULL);
  ASSERT_EQ(order.id_, orderRef);
  ASSERT_EQ(order.side_, Side::Buy);
  ASSERT_EQ(order.quantity_, 1000);
  ASSERT_EQ(order.price_, price);
}

TEST(OrderHandlerTest, AddOrderMPIDMessage) {
  uint8_t buffer[40] = {0};
  buffer[0] = 'F';
  writeBigEndian<uint16_t>(&buffer[1], 42);
  writeBigEndian<uint16_t>(&buffer[3], 55);
  uint64_t timestamp = 0x112233445566ULL;
  uint64_t beTimestamp = boost::endian::native_to_big(timestamp);
  std::memcpy(&buffer[5], reinterpret_cast<uint8_t *>(&beTimestamp) + 2, 6);
  uint64_t orderRef = 9876543210ULL;
  writeBigEndian<uint64_t>(&buffer[11], orderRef);
  buffer[19] = 'B';
  writeBigEndian<uint32_t>(&buffer[20], 1500);
  std::memcpy(&buffer[24], "GOOG    ", 8);
  uint32_t price = 2500000U;
  writeBigEndian<uint32_t>(&buffer[32], price);
  std::memcpy(&buffer[36], "MPID", 4);

  auto order = ITCHOrder<ITCHMessageType::ADD_ORDER>::parse(
      reinterpret_cast<const char *>(buffer));

  ASSERT_EQ(order.stockLocate_, 42);
  ASSERT_EQ(order.timestamp_, timestamp);
  ASSERT_EQ(order.id_, orderRef);
  ASSERT_EQ(order.side_, Side::Buy);
  ASSERT_EQ(order.quantity_, 1500);
  ASSERT_EQ(order.price_, price);
}

TEST(OrderHandlerTest, OrderExecutedMessage) {
  uint8_t buffer[31] = {0};
  buffer[0] = 'E';
  writeBigEndian<uint16_t>(&buffer[1], 101);
  writeBigEndian<uint16_t>(&buffer[3], 202);
  uint64_t timestamp = 0x00123456789ABCULL;
  uint64_t beTimestamp = boost::endian::native_to_big(timestamp);
  std::memcpy(&buffer[5], reinterpret_cast<uint8_t *>(&beTimestamp) + 2, 6);
  uint64_t orderRef = 987654321ULL;
  writeBigEndian<uint64_t>(&buffer[11], orderRef);
  writeBigEndian<uint32_t>(&buffer[19], 500);
  uint64_t matchNum = 555666777ULL;
  writeBigEndian<uint64_t>(&buffer[23], matchNum);

  auto orderExec = ITCHOrder<ITCHMessageType::EXECUTE_ORDER>::parse(
      reinterpret_cast<const char *>(buffer));

  ASSERT_EQ(orderExec.stockLocate_, 101);
  ASSERT_EQ(orderExec.timestamp_, timestamp & 0x0000FFFFFFFFFFFFULL);
  ASSERT_EQ(orderExec.id_, orderRef);
  ASSERT_EQ(orderExec.quantity_, 500);
}

TEST(OrderHandlerTest, OrderExecutedWithPriceMessage) {
  uint8_t buffer[36] = {0};
  buffer[0] = 'C';
  writeBigEndian<uint16_t>(&buffer[1], 111);
  writeBigEndian<uint16_t>(&buffer[3], 222);
  uint64_t timestamp = 0x00ABCDEF123456ULL;
  uint64_t beTimestamp = boost::endian::native_to_big(timestamp);
  std::memcpy(&buffer[5], reinterpret_cast<uint8_t *>(&beTimestamp) + 2, 6);
  uint64_t orderRef = 1122334455ULL;
  writeBigEndian<uint64_t>(&buffer[11], orderRef);
  writeBigEndian<uint32_t>(&buffer[19], 250);
  uint64_t matchNum = 9988776655ULL;
  writeBigEndian<uint64_t>(&buffer[23], matchNum);
  buffer[31] = 'Y';
  uint32_t execPrice = 1525000;
  writeBigEndian<uint32_t>(&buffer[32], execPrice);

  auto orderExecPrice = ITCHOrder<ITCHMessageType::EXECUTE_ORDER>::parse(
      reinterpret_cast<const char *>(buffer));

  ASSERT_EQ(orderExecPrice.stockLocate_, 111);
  ASSERT_EQ(orderExecPrice.timestamp_, timestamp & 0x0000FFFFFFFFFFFFULL);
  ASSERT_EQ(orderExecPrice.id_, orderRef);
  ASSERT_EQ(orderExecPrice.quantity_, 250);
}

TEST(OrderHandlerTest, OrderCancelMessage) {
  uint8_t buffer[23] = {0};
  buffer[0] = 'X';
  writeBigEndian<uint16_t>(&buffer[1], 121);
  writeBigEndian<uint16_t>(&buffer[3], 242);
  uint64_t timestamp = 0x0000FEDCBA9876ULL;
  uint64_t beTimestamp = boost::endian::native_to_big(timestamp);
  std::memcpy(&buffer[5], reinterpret_cast<uint8_t *>(&beTimestamp) + 2, 6);
  uint64_t orderRef = 5566778899ULL;
  writeBigEndian<uint64_t>(&buffer[11], orderRef);
  writeBigEndian<uint32_t>(&buffer[19], 300);

  auto orderCancel = ITCHOrder<ITCHMessageType::CANCEL_ORDER>::parse(
      reinterpret_cast<const char *>(buffer));

  ASSERT_EQ(orderCancel.stockLocate_, 121);
  ASSERT_EQ(orderCancel.timestamp_, timestamp & 0x0000FFFFFFFFFFFFULL);
  ASSERT_EQ(orderCancel.id_, orderRef);
  ASSERT_EQ(orderCancel.numCancelled_, 300);
}

TEST(OrderHandlerTest, OrderDeleteMessage) {
  uint8_t buffer[19] = {0};
  buffer[0] = 'D';
  writeBigEndian<uint16_t>(&buffer[1], 131);
  writeBigEndian<uint16_t>(&buffer[3], 262);
  uint64_t timestamp = 0x0000123456789AULL;
  uint64_t beTimestamp = boost::endian::native_to_big(timestamp);
  std::memcpy(&buffer[5], reinterpret_cast<uint8_t *>(&beTimestamp) + 2, 6);
  uint64_t orderRef = 6677889900ULL;
  writeBigEndian<uint64_t>(&buffer[11], orderRef);

  auto orderDelete = ITCHOrder<ITCHMessageType::DELETE_ORDER>::parse(
      reinterpret_cast<const char *>(buffer));

  ASSERT_EQ(orderDelete.stockLocate_, 131);
  ASSERT_EQ(orderDelete.timestamp_, timestamp & 0x0000FFFFFFFFFFFFULL);
  ASSERT_EQ(orderDelete.id_, orderRef);
}

TEST(OrderHandlerTest, OrderReplaceMessage) {
  uint8_t buffer[35] = {0};
  buffer[0] = 'U';
  writeBigEndian<uint16_t>(&buffer[1], 141);
  writeBigEndian<uint16_t>(&buffer[3], 282);
  uint64_t timestamp = 0x0000A1B2C3D4E5ULL;
  uint64_t beTimestamp = boost::endian::native_to_big(timestamp);
  std::memcpy(&buffer[5], reinterpret_cast<uint8_t *>(&beTimestamp) + 2, 6);
  uint64_t originalOrderRef = 7788990011ULL;
  writeBigEndian<uint64_t>(&buffer[11], originalOrderRef);
  uint64_t newOrderRef = 1122112233ULL;
  writeBigEndian<uint64_t>(&buffer[19], newOrderRef);
  writeBigEndian<uint32_t>(&buffer[27], 1200);
  uint32_t newPrice = 1590000;
  writeBigEndian<uint32_t>(&buffer[31], newPrice);

  auto orderReplace = ITCHOrder<ITCHMessageType::REPLACE_ORDER>::parse(
      reinterpret_cast<const char *>(buffer));

  ASSERT_EQ(orderReplace.stockLocate_, 141);
  ASSERT_EQ(orderReplace.timestamp_, timestamp & 0x0000FFFFFFFFFFFFULL);
  ASSERT_EQ(orderReplace.oldId_, originalOrderRef);
  ASSERT_EQ(orderReplace.newId_, newOrderRef);
  ASSERT_EQ(orderReplace.quantity_, 1200);
  ASSERT_EQ(orderReplace.price_, newPrice);
}

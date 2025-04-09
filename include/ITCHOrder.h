//
// Created by Daniel Suh on 3/27/25.
//

#ifndef SIMPLEORDERBOOK_ORDER_H
#define SIMPLEORDERBOOK_ORDER_H

#include <cstdint>

#include "ITCHMessageType.h"
#include "Side.h"
#include "ITCHParser.h"

template <ITCHMessageType T>
struct ITCHOrder;

/**
 * @brief ITCH Order struct for orders with and without MPID
 */
template <>
struct ITCHOrder<ITCHMessageType::ADD_ORDER> {
  uint16_t stockLocate_{};
  uint64_t timestamp_{};
  uint64_t id_{};
  Side side_{};
  uint32_t quantity_{};
  uint32_t price_{};

  inline ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id,
                   char side, uint32_t volume, uint32_t price);

	inline static ITCHOrder parse(const ITCHParser& itchparser, size_t pos);
};

/**
 * @brief ITCH Order struct for executed orders with and without a price
 */
template <>
struct ITCHOrder<ITCHMessageType::EXECUTE_ORDER> {
  uint16_t stockLocate_{};
  uint64_t timestamp_{};
  uint64_t id_{};
  uint32_t quantity_{};

  inline ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id,
                   uint32_t quantity);

  inline static ITCHOrder parse(const ITCHParser& itchparser, size_t pos);
};

/**
 * @brief ITCH Order struct to cancel an order on the book
 * Will remove a certain number of shares from a LO
 * Different from Order Delete, which completely removes the order from the book
 */
template <>
struct ITCHOrder<ITCHMessageType::CANCEL_ORDER> {
  uint16_t stockLocate_{};
  uint64_t timestamp_{};
  uint64_t id_{};
  uint32_t numCancelled_{};

  inline ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id,
                   uint32_t numCancelled);

	inline static ITCHOrder parse(const ITCHParser& itchparser, size_t pos);
};

/**
 * @brief ITCH Order struct to delete an order on the book
 * Will completely delete a LO from the order book
 */
template <>
struct ITCHOrder<ITCHMessageType::DELETE_ORDER> {
  uint16_t stockLocate_{};
  uint64_t timestamp_{};
  uint64_t id_{};

  inline ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id);

	inline static ITCHOrder parse(const ITCHParser& itchparser, size_t pos);
};

/**
 * @brief ITCH Order struct to update an order on the book
 */
template <>
struct ITCHOrder<ITCHMessageType::REPLACE_ORDER> {
  uint16_t stockLocate_{};
  uint64_t timestamp_{};
  uint64_t oldId_{};
  uint64_t newId_{};
  uint32_t quantity_{};
  uint32_t price_{};

  inline ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t oldId,
                   uint64_t newId, uint32_t quantity, uint32_t price);

	inline static ITCHOrder parse(const ITCHParser& itchparser, size_t pos);
};

#include "ITCHOrder.tpp"

#endif  // SIMPLEORDERBOOK_ORDER_H

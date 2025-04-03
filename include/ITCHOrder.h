//
// Created by Daniel Suh on 3/27/25.
//

#ifndef SIMPLEORDERBOOK_ORDER_H
#define SIMPLEORDERBOOK_ORDER_H

#include "Side.h"
#include "ITCHOrderType.h"
#include "ITCHMessageType.h"
#include <cstdint>

template <ITCHMessageType T>
struct ITCHOrder;

/**
 * @brief ITCH Order struct without a Market Participant Identifier
 */
template <>
struct ITCHOrder<ITCHMessageType::ADD_ORDER> {
	uint16_t stockLocate_{};
	uint64_t timestamp_{};
	uint64_t id_{};
	Side side_{};
	uint32_t volume_{};
	uint32_t price_{};

	inline ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, char side, uint32_t volume, uint32_t price);

	inline static ITCHOrder parse(const char c[]);
};


/**
 * @brief ITCH Order struct with a Market Participant Identifier
 */
template <>
struct ITCHOrder<ITCHMessageType::ADD_ORDER_MPID> {
	uint16_t stockLocate_{};
	uint64_t timestamp_{};
	uint64_t id_{};
	Side side_{};
	uint32_t volume_{};
	uint32_t price_{};

	inline ITCHOrder(uint16_t stockLocate, uint64_t timestamp, uint64_t id, char side, uint32_t volume, uint32_t price);
};

#include "ITCHOrder.tpp"

#endif //SIMPLEORDERBOOK_ORDER_H

#include "Side.h"

/**
 * @brief Converts a character to its respective side
 * @param c The character to convert to the Side
 */
Side charToSide(char c) {
  switch (c) {
    case 'B':
      return Side::Bid;
    case 'S':
      return Side::Ask;
  }
}

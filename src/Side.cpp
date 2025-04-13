#include "Side.h"

Side charToSide(char c) {
  switch (c) {
    case 'B':
      return Side::Bid;
    case 'S':
      return Side::Ask;
  }
}

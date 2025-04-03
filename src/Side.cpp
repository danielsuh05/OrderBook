#include "Side.h"

Side charToSide(char c) {
	switch(c) {
		case 'B':
			return Side::Buy;
		case 'S':
			return Side::Sell;
		default:
			return Side::Error;
	}
}

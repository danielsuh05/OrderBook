//
// Created by Daniel Suh on 3/27/25.
//

#ifndef SIMPLEORDERBOOK_SIDE_H
#define SIMPLEORDERBOOK_SIDE_H

enum class Side {
	Buy = 'B',
	Sell = 'S',
};

Side charToSide(char c);

#endif //SIMPLEORDERBOOK_SIDE_H

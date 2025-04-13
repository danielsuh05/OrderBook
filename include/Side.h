//
// Created by Daniel Suh on 3/27/25.
//

#ifndef SIMPLEORDERBOOK_SIDE_H
#define SIMPLEORDERBOOK_SIDE_H

enum class Side {
  Ask = 'S',
  Bid = 'B',
};

Side charToSide(char c);

#endif  // SIMPLEORDERBOOK_SIDE_H

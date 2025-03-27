//
// Created by Christian Alexander Marrero on 3/26/25.
//

#ifndef VISUALCARD_H
#define VISUALCARD_H

#include "../logic/cards.h"

class VisualCard {
    bool visible;
    ScorableCard *card;

public:
    VisualCard(bool visible);
};



#endif //VISUALCARD_H

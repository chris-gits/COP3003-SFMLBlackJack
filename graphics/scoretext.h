//
// Created by Christian Alexander Marrero on 4/14/25.
//

#ifndef SCORETEXT_H
#define SCORETEXT_H

#include "../logic/cards.h"
#include "SFML/Graphics/Text.hpp"

class ScoreText : public sf::Text {
    public:
    ScoreText(const sf::Font& font, sf::Vector2f position);
    void update(CardCollection& collection);
};

#endif //SCORETEXT_H

//
// Created by Christian Alexander Marrero on 4/14/25.
//

#ifndef CARDSPRITE_H
#define CARDSPRITE_H

#include "SFML/Graphics/Sprite.hpp"
#include "../logic/cards.h"

class CardSprite : public sf::Sprite {
    std::unique_ptr<ScorableCard> card;
    public:
    CardSprite(ScorableCard& card);
    const std::unique_ptr<ScorableCard>& getCard() const;
    void refresh_texture();
};

class CardCollectionSprite : public sf::Drawable {
    sf::Vector2f position;
    CardCollection& card_collection;
    std::vector<CardSprite> card_sprites;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    public:
    CardCollectionSprite(CardCollection& card_collection, sf::Vector2f position);
    void refresh();
};

#endif //CARDSPRITE_H

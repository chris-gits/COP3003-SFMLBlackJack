//
// Created by Christian Alexander Marrero on 4/14/25.
//

#include "cardsprite.h"

#include <iostream>
#include <vector>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"

std::unique_ptr<sf::Texture> cardsSpriteSheet = std::make_unique<sf::Texture>(sf::Texture());
bool cardsSpriteLoaded = cardsSpriteSheet -> loadFromFile("../assets/cardsSpritesheet.png");

const int CARDS_SPRITE_SHEET_X = 48;
const int CARDS_SPRITE_SHEET_Y = 64;
const int CARDS_SPRITE_SHEET_S = 2.5;
const int CARDS_SPRITE_SHEET_P = 8;

// CardSprite
CardSprite::CardSprite(ScorableCard& card) :
    card(std::move(std::make_unique<ScorableCard>(card))),
    sf::Sprite(*cardsSpriteSheet) {
    this -> refresh_texture();
};
const std::unique_ptr<ScorableCard>& CardSprite::getCard() const {
    return this->card;
}
void CardSprite::refresh_texture() {
    int x = 0, y = 0;
    switch (card -> getSuit()) {
        case Suit::Hearts: { y = 0; break; };
        case Suit::Diamonds: { y = 1; break; };
        case Suit::Spades: { y = 2; break; };
        case Suit::Clubs: { y = 3; break; };
    }
    switch (card -> getRank()) {
        case Rank::Ace: { x = 0; break; };
        case Rank::Two: { x = 1; break; };
        case Rank::Three: { x = 2; break; };
        case Rank::Four: { x = 3; break; };
        case Rank::Five: { x = 4; break; };
        case Rank::Six: { x = 5; break; };
        case Rank::Seven: { x = 6; break; };
        case Rank::Eight: { x = 7; break; };
        case Rank::Nine: { x = 8; break; };
        case Rank::Ten: { x = 9; break; };
        case Rank::Jack: { x = 10; break; };
        case Rank::Queen: { x = 11; break; };
        case Rank::King: { x = 12; break; };
    }
    this -> setTextureRect(
        sf::IntRect(
            sf::Vector2i(x*CARDS_SPRITE_SHEET_X, y*CARDS_SPRITE_SHEET_Y),
            sf::Vector2i(CARDS_SPRITE_SHEET_X, CARDS_SPRITE_SHEET_Y)
        )
    );
    this -> setScale(
        sf::Vector2f(CARDS_SPRITE_SHEET_S, CARDS_SPRITE_SHEET_S)
    );
}


// CardCollectionSprite
void CardCollectionSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform.translate(
        this -> position - sf::Vector2f((CARDS_SPRITE_SHEET_X*CARDS_SPRITE_SHEET_S+CARDS_SPRITE_SHEET_P) / 2 * this -> card_sprites.size() - CARDS_SPRITE_SHEET_P, (CARDS_SPRITE_SHEET_Y * CARDS_SPRITE_SHEET_S)/2)
    );
    for (const CardSprite& card_sprite : this -> card_sprites) {
        target.draw(card_sprite, states);
        states.transform.translate(
            sf::Vector2f(
                CARDS_SPRITE_SHEET_X*CARDS_SPRITE_SHEET_S + CARDS_SPRITE_SHEET_P,
                0
                )
            );
    }
}
CardCollectionSprite::CardCollectionSprite(CardCollection &card_collection, sf::Vector2f position) :
    card_collection(card_collection),
    card_sprites(std::vector<CardSprite>()),
    position(position){};
void CardCollectionSprite::refresh() {
    this -> card_sprites.clear();
    for (const std::unique_ptr<ScorableCard>& card_sprite : this -> card_collection.getCards()) {
        this -> card_sprites.push_back(
            CardSprite(*card_sprite)
        );
    }
}

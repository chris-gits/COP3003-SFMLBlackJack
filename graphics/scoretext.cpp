//
// Created by Christian Alexander Marrero on 4/14/25.
//

#include "scoretext.h"
#include "SFML/Graphics/Font.hpp"

ScoreText::ScoreText(const sf::Font& font, sf::Vector2f position) :
    sf::Text(font) {
    this -> setPosition(position);
    this -> setOutlineThickness(2.0);
    this -> setStyle(sf::Text::Bold);
}

void ScoreText::update(CardCollection& collection) {
    std::vector<int> scores = collection.getValidScores();
    std::string updated_text = "";
    if (scores.size() > 0) {
        for (int i = 0; i < scores.size(); i++) {
            updated_text += std::to_string(scores[i]);
            if (i != scores.size() - 1) {
                updated_text += ", ";
            }
        }
    } else {
        updated_text = "Bust";
    }
    this -> setString(updated_text);
    auto dimensions = this -> getGlobalBounds().size;
    this -> setOrigin(
        sf::Vector2f(dimensions.x/2,dimensions.y/2)
    );
}

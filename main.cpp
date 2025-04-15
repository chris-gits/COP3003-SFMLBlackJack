#include <iostream>
#include <set>
#include <vector>

#include "logic/cards.h"
#include "logic/blackjack.h"
#include "graphics/cardsprite.h"
#include "graphics/scoretext.h"

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Text.hpp"

int main() {
    // Initialize the blackJack game.
    Blackjack blackjack;
    blackjack.start(50);

    // Create graphical elements.
    sf::Font font("../assets/Texturina/Texturina-VariableFont_opsz,wght.ttf");
    CardCollectionSprite dealers_hand_sprite(blackjack.getDealersHand(), sf::Vector2f(400, 100));
    CardCollectionSprite players_hand_sprite(blackjack.getPlayersHand(), sf::Vector2f(400, 500));
    ScoreText dealers_score_text(font, sf::Vector2f(400, 200));
    ScoreText players_score_text(font, sf::Vector2f(400, 400));
    sf::RectangleShape turn_indicator(sf::Vector2f(50, 50));

    // Define event flag & cooldown.
    bool awaiting_player = false;
    float event_cooldown = 0;

    // Create & configure the window.
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    window.setTitle("SFML BlackJack");

    // Create the Clock & DeltaTime
    sf::Clock clock;
    float delta_time;

    // Primary loop that continues as long as the window is open.
    while (window.isOpen())
    {
        // Iterate each window event through a poll loop.
        while (const std::optional event = window.pollEvent())
        {
            // Close window if event matches so.
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Handle input for player's actions.
        if (awaiting_player) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::H)) {
                blackjack.hit();
                turn_indicator.setFillColor(sf::Color::Red);
                awaiting_player = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
                blackjack.stand();
                turn_indicator.setFillColor(sf::Color::Red);
                awaiting_player = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
                blackjack.doubleDown();
                turn_indicator.setFillColor(sf::Color::Red);
                awaiting_player = false;
            }
        }

        // Consume (poll) events when ready.
        if (!awaiting_player && event_cooldown <= 0) {
            BlackjackEvent blackjack_event = blackjack.nextEvent();
            switch (blackjack_event) {
                case BlackjackEvent::StartRound: {
                    std::cout << "EVENT STARTROUND\n";
                    break;
                }
                case BlackjackEvent::PlayerTurn: {
                    std::cout << "EVENT PLAYERTURN\n";
                    turn_indicator.setFillColor(sf::Color::Green);
                    awaiting_player = true;
                    break;
                }
                case BlackjackEvent::DealerTurn: {
                    std::cout << "EVENT DEALERTURN\n";
                    break;
                }
                case BlackjackEvent::PlayerHit: {
                    std::cout << "EVENT PLAYERHIT\n";
                    players_hand_sprite.refresh();
                    players_score_text.update(blackjack.getPlayersHand());
                    break;
                }
                case BlackjackEvent::DealerHit: {
                    std::cout << "EVENT DEALERHIT\n";
                    dealers_hand_sprite.refresh();
                    dealers_score_text.update(blackjack.getDealersHand());
                    break;
                }
                case BlackjackEvent::ResolveGame: {
                    std::cout << "EVENT RESOLVEGAME\n";
                    break;
                }
                case BlackjackEvent::PlayerBust: {
                    std::cout << "EVENT PLAYERBUST\n";
                    std::cout << "\n---- Player Busted.\n\n";
                    break;
                }
                case BlackjackEvent::DealerBust: {
                    std::cout << "EVENT DEALERBUST\n";
                    std::cout << "\n---- Dealer Busted.\n\n";
                    break;
                }
                case BlackjackEvent::Draw: {
                    std::cout << "EVENT DRAW\n";
                    std::cout << "Draw.\n";
                    break;
                }
                default: { // BlackjackEvent::EndRound
                    std::cout << "EVENT ENDROUND\n";
                    blackjack.start(50);
                    break;
                }
            }
            event_cooldown += 0.125;
        }

        // Update delta_time with clock.restart() & subtract
        // it from event_cooldown.
        delta_time = float(clock.restart().asSeconds());
        if (event_cooldown - delta_time > 0) {
            event_cooldown -= delta_time;
        } else {
            event_cooldown = 0;
        }

        // Clear the window by applying background color.
        window.clear(sf::Color(15, 63, 20));

        // Draw each graphical element.
        window.draw(dealers_hand_sprite);
        window.draw(players_hand_sprite);
        window.draw(players_score_text);
        window.draw(dealers_score_text);
        window.draw(turn_indicator);

        // Render the current buffer to the window.
        window.display();
    }

    // EOP
    std::cout << "Program ended.\n";
}

#include <iostream>
#include <set>
#include <vector>

#include "logic/cards.h"
#include "logic/blackjack.h"
#include "graphics/cardsprite.h"
#include "graphics/scoretext.h"

#include "SFML/Graphics.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Audio.hpp"

int main() {
    // Define event flag & cooldown.
    bool awaiting_player = false;
    float event_cooldown = 0.25;

    // Round variables.
    int intial_bet = 50;
    int balance = 0;

    // Game-wide variables.
    int wins = 0;
    int losses = 0;
    int draws = 0;
    int round = 0;

    // Initialize the blackJack game.
    Blackjack blackjack;
    blackjack.start(intial_bet);

    // Create & configure the window.
    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "SFML BlackJack"
        // , sf::Style::Titlebar | sf::Style::Close
        );

    // Create & configure graphical elements.
    sf::Font font("../assets/Texturina/Texturina-VariableFont_opsz,wght.ttf");
    CardCollectionSprite dealers_hand_sprite(blackjack.getDealersHand(), sf::Vector2f(400, 100));
    CardCollectionSprite players_hand_sprite(blackjack.getPlayersHand(), sf::Vector2f(400, 500));
    ScoreText dealers_score_text(font, sf::Vector2f(400, 200));
    ScoreText players_score_text(font, sf::Vector2f(400, 400));
    // sf::RectangleShape turn_indicator(sf::Vector2f(50, 50));
    sf::Text bet_text(font, "Bet: $0");
    bet_text.setPosition(sf::Vector2f(8, 525));
    bet_text.setOutlineThickness(2);
    bet_text.setCharacterSize(25);
    sf::Text balance_text(font, "Balance: $0");
    balance_text.setPosition(sf::Vector2f(8, 558));
    balance_text.setOutlineThickness(2);
    sf::Text motivational_text(font, "Remember, 90%\nof gamblers\nquit before\nthey hit it big...", 15);
    motivational_text.setStyle(sf::Text::Italic | sf::Text::Bold);
    motivational_text.setFillColor(sf::Color(229,  225, 4));
    motivational_text.setOutlineThickness(2);
    motivational_text.setOutlineColor(sf::Color::Black);
    motivational_text.setPosition(sf::Vector2f(8, 8));
    sf::Text win_text(font, "Wins: 0", 25);
    win_text.setOutlineThickness(2);
    win_text.setPosition(sf::Vector2f(650, 8));
    sf::Text losses_text(font, "Losses: 0", 25);
    losses_text.setOutlineThickness(2);
    losses_text.setPosition(sf::Vector2f(650, 38));
    sf::Text draws_text(font, "Draws: 0", 25);
    draws_text.setOutlineThickness(2);
    draws_text.setPosition(sf::Vector2f(650, 68));
    sf::Text keybinds_text(font, "H: Hit\nS: Stand\nD: Double", 25);
    keybinds_text.setOutlineThickness(2);
    keybinds_text.setPosition(sf::Vector2f(660, 475));
    sf::Texture you_won_image = sf::Texture("../assets/youWon.png");
    sf::Sprite you_won_sprite(you_won_image);
    you_won_sprite.setScale(sf::Vector2f(1.25, 1.25));
    you_won_sprite.setPosition(sf::Vector2f(
        float((you_won_image.getSize().x) * -1.25 + 800) / 2.0,
        float((you_won_image.getSize().y) * -1.25 + 600) / 2.0
    ));
    bool show_you_won_image = false;
    sf::Texture you_lost_image = sf::Texture("../assets/youLost.png");
    sf::Sprite you_lost_sprite(you_lost_image);
    you_lost_sprite.setScale(sf::Vector2f(0.75, 0.75));
    you_lost_sprite.setPosition(sf::Vector2f(
        float((you_lost_image.getSize().x) * -0.75 + 800) / 2.0,
        float((you_lost_image.getSize().y) * -0.75 + 600) / 2.0
    ));
    bool show_you_lost_image = false;
    sf::Texture game_drawed_image = sf::Texture("../assets/draw.png");
    sf::Sprite game_drawed_sprite(game_drawed_image);
    game_drawed_sprite.setScale(sf::Vector2f(0.75, 0.75));
    game_drawed_sprite.setPosition(sf::Vector2f(
        float((game_drawed_image.getSize().x) * -0.75 + 800) / 2.0,
        float((game_drawed_image.getSize().y) * -0.75 + 600) / 2.0
    ));
    bool show_game_drawed_image = false;
    sf::Texture blackjack_image = sf::Texture("../assets/blackjack.png");
    sf::Sprite blackjack_sprite(blackjack_image);
    blackjack_sprite.setScale(sf::Vector2f(1, 1));
    blackjack_sprite.setPosition(sf::Vector2f(
        float((blackjack_image.getSize().x) * -1 + 800) / 2.0,
        float((blackjack_image.getSize().y) * -1 + 600) / 2.0
    ));
    bool show_blackjack_image = false;
    sf::Texture hotline_image = sf::Texture("../assets/hotline.png");
    sf::Sprite hotline_sprite(hotline_image);
    hotline_sprite.setScale(sf::Vector2f(0.75, 0.75));
    hotline_sprite.setPosition(sf::Vector2f(
        float((hotline_image.getSize().x) * -0.75 + 800) / 2.0,
        float((hotline_image.getSize().y) * -0.75 + 600) / 2.0
    ));
    bool show_hotline_image = false;

    // Audio
    sf::SoundBuffer sound_buffer_music("../assets/backgroundMusic.wav");
    sf::Sound sound_music(sound_buffer_music);
    sound_music.setLooping(true);
    sound_music.play();
    sf::SoundBuffer sound_buffer_win("../assets/winSound.wav");
    sf::Sound sound_win(sound_buffer_win);
    sf::SoundBuffer sound_buffer_loss("../assets/lossSound.wav");
    sf::Sound sound_loss(sound_buffer_loss);
    sf::SoundBuffer sound_buffer_draw("../assets/drawSound.wav");
    sf::Sound sound_draw(sound_buffer_draw);
    sf::SoundBuffer sound_buffer_click("../assets/click.wav");
    sf::Sound sound_click(sound_buffer_click);
    sf::SoundBuffer sound_buffer_blackjack("../assets/blackjackSound.wav");
    sf::Sound sound_blackjack(sound_buffer_blackjack);

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
                // turn_indicator.setFillColor(sf::Color::Red);
                awaiting_player = false;
                sound_click.play();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
                blackjack.stand();
                // turn_indicator.setFillColor(sf::Color::Red);
                awaiting_player = false;
                sound_click.play();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
                blackjack.doubleDown();
                // turn_indicator.setFillColor(sf::Color::Red);
                awaiting_player = false;
                sound_click.play();
            }
        }

        // Consume (poll) events when ready.
        if (!awaiting_player && event_cooldown <= 0) {
            BlackjackEvent blackjack_event = blackjack.nextEvent();
            switch (blackjack_event) {
                case BlackjackEvent::StartRound: {
                    show_you_won_image = false;
                    show_you_lost_image = false;
                    show_game_drawed_image = false;
                    show_blackjack_image = false;
                    round += 1;
                    bet_text.setString("Bet: $" + std::to_string(blackjack.getBet()));
                    break;
                }
                case BlackjackEvent::PlayerTurn: {
                    // turn_indicator.setFillColor(sf::Color::Green);
                    awaiting_player = true;
                    break;
                }
                case BlackjackEvent::DealerTurn: {
                    break;
                }
                case BlackjackEvent::PlayerHit: {
                    players_hand_sprite.refresh();
                    players_score_text.update(blackjack.getPlayersHand());
                    break;
                }
                case BlackjackEvent::DealerHit: {
                    dealers_hand_sprite.refresh();
                    dealers_score_text.update(blackjack.getDealersHand());
                    break;
                }
                case BlackjackEvent::PlayerDoubleDown: {;
                    bet_text.setString("Bet: $" + std::to_string(blackjack.getBet()));
                    break;
                }
                case BlackjackEvent::ResolveGame: {
                    break;
                }
                case BlackjackEvent::PlayerBust: {
                    sound_loss.play();
                    show_you_lost_image = true;
                    balance -= blackjack.getBet();
                    losses += 1;
                    event_cooldown += 1;
                    break;
                }
                case BlackjackEvent::DealerBust: {
                    sound_win.play();
                    show_you_won_image = true;
                    balance += blackjack.getBet();
                    wins += 1;
                    event_cooldown += 1;
                    break;
                }
                case BlackjackEvent::Draw: {
                    sound_draw.play();
                    show_game_drawed_image = true;
                    draws += 1;
                    event_cooldown += 1;
                    break;
                }
                case BlackjackEvent::PlayerBlackjack: {
                    show_blackjack_image = true;
                    sound_blackjack.play();
                    balance += blackjack.getBet() * 1.5;
                    wins += 1;
                    event_cooldown += 3;
                    break;
                }
                default: { // BlackjackEvent::EndRound
                    if (balance > -500) {
                        balance_text.setString("Balance: $" + std::to_string(balance));
                    } else {
                        balance_text.setString("Balance: don't worry...");
                    }
                    if (round == 20) { show_hotline_image = true; }
                    win_text.setString("Wins: " + std::to_string(wins));
                    if (losses - wins < 10) { losses_text.setString("Losses: " + std::to_string(losses)); }
                    else {losses_text.setString("Keep going.");}
                    draws_text.setString("Draws: " + std::to_string(draws));
                    blackjack.start(intial_bet);
                    break;
                }
            }
            event_cooldown += 0.1250;
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
        if (losses - wins >= 5 && balance < 0) {
            window.draw(motivational_text);
        }
        window.draw(win_text);
        window.draw(losses_text);
        window.draw(draws_text);
        window.draw(bet_text);
        window.draw(balance_text);
        if (awaiting_player) { window.draw(keybinds_text); }
        if (show_hotline_image) { window.draw(hotline_sprite); }
        if (show_you_won_image) { window.draw(you_won_sprite); }
        if (show_you_lost_image) { window.draw(you_lost_sprite); }
        if (show_game_drawed_image) { window.draw(game_drawed_sprite); }
        if (show_blackjack_image) { window.draw(blackjack_sprite); }

        // Render the current buffer to the window.
        window.display();
    }

    // EOP
    std::cout << "Program ended.\n";
}

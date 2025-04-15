#include "blackjack.h"
#include "cards.h"
#include <stack>
#include <optional>
#include <algorithm>
#include <iostream>

#include "SFML/Window/Event.hpp"

// Constructor
Blackjack::Blackjack() :
    players_hand(CardCollection()),
    dealers_hand(CardCollection()),
    game_deck(CardCollection()),
    bet(0),
    events_queue(std::queue<BlackjackEvent>()),
    player_standing(false) {}

// Game Initializer
void Blackjack::start(int initial_bet) {
    this -> bet = initial_bet;
    events_queue.push(BlackjackEvent::StartRound);
};

// Actions
void Blackjack::hit() {
    this -> events_queue.push(BlackjackEvent::PlayerHit);
};
void Blackjack::stand() {
    this -> player_standing = true;
    this -> events_queue.push(BlackjackEvent::DealerTurn);
};
void Blackjack::doubleDown() {
    this -> bet *= 2;
    this -> events_queue.push(BlackjackEvent::DealerTurn);
};

// Event Consumer
BlackjackEvent Blackjack::nextEvent() {
    BlackjackEvent event = events_queue.front();
    events_queue.pop();
    switch (event) {
        case BlackjackEvent::StartRound: { this->eventStartRound(); break; }
        case BlackjackEvent::PlayerTurn: { this->eventPlayerTurn(); break; }
        case BlackjackEvent::DealerTurn: { this->eventDealerTurn(); break; }
        case BlackjackEvent::PlayerHit: { this->eventPlayerHit(); break; }
        case BlackjackEvent::DealerHit: { this->eventDealerHit(); break; }
        case BlackjackEvent::ResolveGame: { this->eventResolveGame(); break; }
        case BlackjackEvent::PlayerBust: { this->eventPlayerBust(); break; }
        case BlackjackEvent::DealerBust: { this->eventDealerBust(); break; }
        case BlackjackEvent::Draw: { this->eventDraw(); break; }
    }
    return event;
};

void Blackjack::checkForBusts() {
    std::vector<int> players_valid_scores = this -> players_hand.getValidScores();
    std::vector<int> dealers_valid_scores = this -> dealers_hand.getValidScores();
    if (dealers_valid_scores.size() == 0) {
        this -> events_queue.push(BlackjackEvent::DealerBust);
    }
    if (players_valid_scores.size() == 0) {
        this -> events_queue.push(BlackjackEvent::PlayerBust);
    }
};

// Event Handlers
void Blackjack::eventStartRound() {
    // This event should prepare the class for
    // the rest of the game.
    this -> players_hand.clear();
    this -> dealers_hand.clear();
    this -> game_deck.clear();
    generateStandardDeck(game_deck);
    game_deck.shuffle();
    this -> player_standing = false;
    this -> events_queue.push(BlackjackEvent::DealerHit);
    this -> events_queue.push(BlackjackEvent::DealerHit);
    this -> events_queue.push(BlackjackEvent::PlayerHit);
    this -> events_queue.push(BlackjackEvent::PlayerHit);
    this -> events_queue.push(BlackjackEvent::PlayerTurn);
};
void Blackjack::eventPlayerTurn() {
    // This event is used to allow whatever
    // external procedure is running to choose
    // what action the player wishes to perform.
    // Thus, this event does not have any
    // internal procedure.
};
void Blackjack::eventDealerTurn() {
    if (this -> dealers_hand.getCount() == 0) {
        this -> events_queue.push(BlackjackEvent::DealerHit);
        return;
    }
    std::vector<int> valid_dealer_scores = this -> dealers_hand.getValidScores();
    int lowest_valid_dealer_score = *std::min_element(
        valid_dealer_scores.begin(),
        valid_dealer_scores.end()
        );
    if (lowest_valid_dealer_score < 17) {
        this -> events_queue.push(BlackjackEvent::DealerHit);
    } else {
        this -> events_queue.push(BlackjackEvent::ResolveGame);
    }
};
void Blackjack::eventPlayerHit() {
    std::optional<std::unique_ptr<ScorableCard>> pulled_card = this -> game_deck.pull();
    if (pulled_card.has_value()) {
        this -> players_hand.push(std::move(*pulled_card));
        this -> checkForBusts();
        if (events_queue.empty()) {
            std::vector<int> new_valid_scores = this -> players_hand.getValidScores();
            int highest_valid_score = *std::max_element(new_valid_scores.begin(), new_valid_scores.end());
            if (highest_valid_score == 21) {
                this -> player_standing = true;
                this -> events_queue.push(BlackjackEvent::DealerTurn);
            } else if (!this -> player_standing) {
                this -> events_queue.push(BlackjackEvent::PlayerTurn);
            } else {
                this -> events_queue.push(BlackjackEvent::DealerTurn);
            }
        }
    } else {
        this -> events_queue.push(BlackjackEvent::ResolveGame);
    }
};
void Blackjack::eventDealerHit() {
    std::optional<std::unique_ptr<ScorableCard>> pulled_card = this -> game_deck.pull();
    if (pulled_card.has_value()) {
        this -> dealers_hand.push(std::move(*pulled_card));
        this -> checkForBusts();
        if (events_queue.empty()) {
            if (this->player_standing) {
                this -> events_queue.push(BlackjackEvent::DealerTurn);
            } else  {
                this -> events_queue.push(BlackjackEvent::PlayerTurn);
            }
        }
    } else {
        this -> events_queue.push(BlackjackEvent::ResolveGame);
    }
};
void Blackjack::eventResolveGame() {
    std::vector<int> player_valid_scores = this -> players_hand.getValidScores();
    std::vector<int> dealer_valid_scores = this -> dealers_hand.getValidScores();
    int player_highest_score = *std::max_element(player_valid_scores.begin(), player_valid_scores.end());
    int dealer_highest_score = *std::max_element(dealer_valid_scores.begin(), dealer_valid_scores.end());
    if (player_highest_score == dealer_highest_score) {
        this -> events_queue.push(BlackjackEvent::Draw);
    } else if (player_highest_score > dealer_highest_score) {
        this -> events_queue.push(BlackjackEvent::DealerBust);
    } else if (player_highest_score < dealer_highest_score) {
        this -> events_queue.push(BlackjackEvent::PlayerBust);
    }
};
void Blackjack::eventPlayerBust() {
    this -> events_queue.push(BlackjackEvent::EndRound);
};
void Blackjack::eventDealerBust() {
    this -> events_queue.push(BlackjackEvent::EndRound);
};
void Blackjack::eventDraw() {
    this -> events_queue.push(BlackjackEvent::EndRound);
};
void Blackjack::eventEndRound() {
    // Cosmetic.
};

CardCollection& Blackjack::getPlayersHand() {
    return this -> players_hand;
};
CardCollection& Blackjack::getDealersHand() {
    return this -> dealers_hand;
}
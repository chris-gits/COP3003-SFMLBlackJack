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
    this -> events_queue.push(BlackjackEvent::PlayerDoubleDown);
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
        case BlackjackEvent::PlayerDoubleDown: { this->eventPlayerDoubleDown(); break; }
        case BlackjackEvent::ResolveGame: { this->eventResolveGame(); break; }
        case BlackjackEvent::PlayerBust: { this->eventPlayerBust(); break; }
        case BlackjackEvent::DealerBust: { this->eventDealerBust(); break; }
        case BlackjackEvent::PlayerBlackjack: { this->eventPlayerBlackjack(); break; }
        case BlackjackEvent::Draw: { this->eventDraw(); break; }
    }
    return event;
};

void Blackjack::checkForBusts() {
    std::cout << "Event - checkForBusts\n";
    std::vector<int> player_valid_scores = this -> players_hand.getValidScores();
    std::vector<int> dealer_valid_scores = this -> dealers_hand.getValidScores();
    if (player_valid_scores.size() != 0 && dealer_valid_scores.size() != 0) {
        int player_highest_score = *std::max_element(player_valid_scores.begin(), player_valid_scores.end());
        int dealer_highest_score = *std::max_element(dealer_valid_scores.begin(), dealer_valid_scores.end());
        if (dealer_highest_score == 21 && dealer_highest_score != player_highest_score) {
            this -> events_queue = std::queue<BlackjackEvent>();
            this -> events_queue.push(BlackjackEvent::PlayerBust);
            return;
        } else if (player_highest_score == 21 && dealer_highest_score != player_highest_score) {
            this -> events_queue = std::queue<BlackjackEvent>();
            this -> events_queue.push(BlackjackEvent::PlayerBlackjack);
        } else if (player_highest_score == dealer_highest_score == 21) {
            this -> events_queue = std::queue<BlackjackEvent>();
            this -> events_queue.push(BlackjackEvent::Draw);
        }
    }
    if (dealer_valid_scores.size() == 0) {
        if (*std::max_element(player_valid_scores.begin(), player_valid_scores.end()) == 21) {
            this -> events_queue.push(BlackjackEvent::PlayerBlackjack);
            return;
        } else {
            this -> events_queue.push(BlackjackEvent::DealerBust);
            return;
        }
    }
    if (player_valid_scores.size() == 0) {
        this -> events_queue.push(BlackjackEvent::PlayerBust);
        return;
    }
};

// Event Handlers
void Blackjack::eventStartRound() {
    std::cout << "Event - eventStartRound\n";
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
    std::cout << "Event - eventPlayerTurn\n";
    // This event is used to allow whatever
    // external procedure is running to choose
    // what action the player wishes to perform.
    // Thus, this event does not have any
    // internal procedure.
};
void Blackjack::eventDealerTurn() {
    std::cout << "Event - eventDealerTurn\n";
    if (this -> dealers_hand.getCount() == 0) {
        this -> events_queue.push(BlackjackEvent::DealerHit);
        return;
    }
    std::vector<int> valid_dealer_scores = this -> dealers_hand.getValidScores();
    int highest_valid_dealer_score = *std::max_element(
        valid_dealer_scores.begin(),
        valid_dealer_scores.end()
        );
    if (highest_valid_dealer_score < 17) {
        this -> events_queue.push(BlackjackEvent::DealerHit);
    } else {
        this -> events_queue.push(BlackjackEvent::ResolveGame);
    }
};
void Blackjack::eventPlayerHit() {
    std::cout << "Event - eventPlayerHit\n";
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
    std::cout << "Event - eventDealerHit\n";
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
    std::cout << "Event - eventResolveGame\n";
    std::vector<int> player_valid_scores = this -> players_hand.getValidScores();
    std::vector<int> dealer_valid_scores = this -> dealers_hand.getValidScores();
    int player_highest_score = *std::max_element(player_valid_scores.begin(), player_valid_scores.end());
    int dealer_highest_score = *std::max_element(dealer_valid_scores.begin(), dealer_valid_scores.end());
    if (player_highest_score == dealer_highest_score) {
        this -> events_queue.push(BlackjackEvent::Draw);
    } else if (player_highest_score > dealer_highest_score) {
        if (player_highest_score == 21) {
            this -> events_queue.push(BlackjackEvent::PlayerBlackjack);
        } else {
            this -> events_queue.push(BlackjackEvent::DealerBust);
        }
    } else if (player_highest_score < dealer_highest_score) {
        this -> events_queue.push(BlackjackEvent::PlayerBust);
    }
};
void Blackjack::eventPlayerBust() {
    std::cout << "Event - eventPlayerBust\n";
    this -> events_queue.push(BlackjackEvent::EndRound);
};
void Blackjack::eventDealerBust() {
    std::cout << "Event - eventDealerBust\n";
    this -> events_queue.push(BlackjackEvent::EndRound);
};
void Blackjack::eventDraw() {
    std::cout << "Event - eventDraw\n";
    this -> events_queue.push(BlackjackEvent::EndRound);
};
void Blackjack::eventEndRound() {
    std::cout << "Event - eventEndRound\n";
    // Cosmetic.
};
void Blackjack::eventPlayerDoubleDown() {
    std::cout << "Event - eventPlayerDoubleDown\n";
    this -> bet *= 2;
    this -> events_queue.push(BlackjackEvent::PlayerHit);
    this -> player_standing = true;
}
void Blackjack::eventPlayerBlackjack() {
    std::cout << "Event - eventPlayerBlackjack\n";
    this -> events_queue.push(BlackjackEvent::EndRound);
}
int Blackjack::getBet() const {
    return this -> bet;
}

CardCollection& Blackjack::getPlayersHand() {
    return this -> players_hand;
};
CardCollection& Blackjack::getDealersHand() {
    return this -> dealers_hand;
}
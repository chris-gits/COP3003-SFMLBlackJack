#include "blackjack.h"
#include "cards.h"
#include <optional>

// Constructor
BlackJack::BlackJack() :
    player_hands(std::vector<CardCollection>()),
    dealer_hand(CardCollection()),
    deck(CardCollection()),
    bet(0),
    action_queue(std::queue<BlackjackAction>()),
    player_current_hand(nullptr) {};
void BlackJack::setup() {
    player_hands.clear();
    player_hands.push_back(CardCollection());
    dealer_hand.clear();
    deck.clear();
    generateStandardDeck(deck);
    action_queue = std::queue<BlackjackAction>();
};
void BlackJack::checkForBusts() {
    if ((*player_current_hand).getValidScores().empty()) {
        action_queue = std::queue<BlackjackAction>();
        action_queue.push(BlackjackAction::PlayerBust);
        return;
    };
    if (dealer_hand.getValidScores().empty()) {
        action_queue = std::queue<BlackjackAction>();
        action_queue.push(BlackjackAction::DealerBust);
    }
};
void BlackJack::playerHit() {
    std::optional<std::unique_ptr<ScorableCard>> pulled_card = deck.pull();
    if (!pulled_card.has_value()) {
        action_queue = std::queue<BlackjackAction>();
        action_queue.push(BlackjackAction::DeckEmpty);
        return;
    }
    (*player_current_hand).push(std::move(pulled_card.value()));
    checkForBusts();
};
void BlackJack::dealerHit() {
    std::optional<std::unique_ptr<ScorableCard>> pulled_card = deck.pull();
    if (!pulled_card.has_value()) {
        action_queue = std::queue<BlackjackAction>();
        action_queue.push(BlackjackAction::DeckEmpty);
        return;
    }
    dealer_hand.push(std::move(pulled_card.value()));
    checkForBusts();
};
void BlackJack::playerSplit() {};
void BlackJack::playerDoubleDown() {};
void BlackJack::playerSurrender() {};
void BlackJack::playerBust() {};
void BlackJack::dealerBust() {};
void BlackJack::playerWin() {};
void BlackJack::dealerWin() {};

void BlackJack::surrender() { action_queue.push(BlackjackAction::PlayerSurrender); };
void BlackJack::doubleDown() { action_queue.push(BlackjackAction::PlayerDoubleDown); };
void BlackJack::split() { action_queue.push(BlackjackAction::PlayerSplit); };
void BlackJack::hit() { action_queue.push(BlackjackAction::PlayerHit); };
void BlackJack::stand() { action_queue.push(BlackjackAction::PlayerStand); };
BlackjackAction BlackJack::nextEvent() {
    BlackjackAction action = action_queue.front();
    action_queue.pop();
    switch (action) {
        case BlackjackAction::PlayerHit: playerHit(); break;
        case BlackjackAction::DealerHit: dealerHit(); break;
        case BlackjackAction::PlayerSplit: playerSplit(); break;
        case BlackjackAction::PlayerDoubleDown: playerDoubleDown(); break;
        case BlackjackAction::PlayerSurrender: playerSurrender(); break;
        default: return BlackjackAction::AwaitAction;
    }
    return action;
};
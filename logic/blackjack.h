#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <queue>
#include <stack>

#include "cards.h"

enum BlackjackEvent {
    // Startup
    StartRound,

    // Turns
    PlayerTurn,
    DealerTurn,

    // Cards
    PlayerHit,
    DealerHit,

    // Double
    PlayerDoubleDown,

    // Resolve
    ResolveGame,

    // Busts
    PlayerBust,
    DealerBust,

    // Blackjack
    PlayerBlackjack,

    // Draw
    Draw,

    // End
    EndRound,
};

class Blackjack {
    // Card Collections
    CardCollection players_hand;
    CardCollection dealers_hand;
    CardCollection game_deck;

    // Bet
    int bet;

    // Standing
    bool player_standing;

    // Events
    std::queue<BlackjackEvent> events_queue;

    // Checks
    void checkForBusts();

    // Event Handlers
    void eventStartRound();
    void eventPlayerTurn();
    void eventDealerTurn();
    void eventPlayerHit();
    void eventDealerHit();
    void eventResolveGame();
    void eventPlayerBust();
    void eventDealerBust();
    void eventDraw();
    void eventEndRound();
    void eventPlayerDoubleDown();
    void eventPlayerBlackjack();
public:
    // Constructor
    Blackjack();

    // Game Initializer
    void start(int initial_bet);

    // Actions
    void hit();
    void stand();
    void doubleDown();

    // Event Consumer
    BlackjackEvent nextEvent();

    // Accessors
    CardCollection& getPlayersHand();
    CardCollection& getDealersHand();
    int getBet() const;
};

#endif
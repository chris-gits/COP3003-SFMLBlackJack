#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <queue>

#include "cards.h"

enum BlackjackAction {
    AwaitAction,
    PlayerHit,
    PlayerBust,
    PlayerWin,
    PlayerDoubleDown,
    PlayerSurrender,
    PlayerStand,
    PlayerSplit,
    DealerHit,
    DealerBust,
    DeckEmpty,
};

class BlackJack {
    std::queue<BlackjackAction> action_queue;
    std::vector<CardCollection> player_hands;
    CardCollection* player_current_hand;
    CardCollection dealer_hand;
    CardCollection deck;
    int bet;
private:
    void checkForBusts();
    void playerHit();
    void dealerHit();
    void playerSplit();
    void playerDoubleDown();
    void playerSurrender();
    void playerBust();
    void dealerBust();
    void playerWin();
    void dealerWin();
public:
    BlackJack();
    void setup(); // Clear players and dealers hands, regenerate deck, pull first cards
    void surrender();
    void doubleDown();
    void split();
    void hit();
    void stand();
    std::vector<CardCollection> getPlayerHands();
    CardCollection getDealerHand();
    void setBet(int bet);
    BlackjackAction nextEvent();
};

#endif
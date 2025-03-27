#ifndef CARDS_H
#define CARDS_H

#include <random>
#include <set>

enum Suit { Spades, Diamonds, Clubs, Hearts };
enum Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

class ScorableCard {
    Suit suit;
    Rank rank;
    std::vector<int> scores;
public:
    ScorableCard(Suit suit, Rank rank, std::vector<int> scores);
    ScorableCard(Suit suit, Rank rank);
    Suit getSuit() const;
    Rank getRank() const;
    std::vector<int> getScores() const;
    std::string toString();
};

class CardCollection {
    std::vector<std::unique_ptr<ScorableCard>> cards;
public:
    CardCollection();
    const std::vector<std::unique_ptr<ScorableCard>>& getCards();
    void push(std::unique_ptr<ScorableCard> card);
    std::optional<std::unique_ptr<ScorableCard>> pull();
    void shuffle();
    int getCount();
    std::vector<int> getScores();
    std::set<int> getValidScores();
    std::vector<int> addScores();
    void clear();
};

void generateStandardDeck(CardCollection &collection);

#endif
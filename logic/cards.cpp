#include "cards.h"

#include <iostream>
#include <set>

ScorableCard::ScorableCard(Suit suit, Rank rank, std::vector<int> scores) :
    suit(suit),
    rank(rank),
    scores(std::move(scores)) {}
ScorableCard::ScorableCard(Suit suit, Rank rank)  : suit(suit), rank(rank) {
    this -> scores =
        (rank == Rank::Ace) ? std::vector<int>{1,11}
        : (rank == Rank::Two) ? std::vector<int>{2}
        : (rank == Rank::Three) ? std::vector<int>{3}
        : (rank == Rank::Four) ? std::vector<int>{4}
        : (rank == Rank::Five) ? std::vector<int>{5}
        : (rank == Rank::Six) ? std::vector<int>{6}
        : (rank == Rank::Seven) ? std::vector<int>{7}
        : (rank == Rank::Eight) ? std::vector<int>{8}
        : (rank == Rank::Nine) ? std::vector<int>{9}
        : (rank == Rank::Ten) ? std::vector<int>{10}
        : (rank == Rank::Jack) ? std::vector<int>{10}
        : (rank == Rank::Queen) ? std::vector<int>{10}
        : (rank == Rank::King) ? std::vector<int>{10}
        : std::vector<int>{0};
}
Suit ScorableCard::getSuit() const {
    return this -> suit;
}
Rank ScorableCard::getRank() const {
    return this -> rank;
}
std::vector<int> ScorableCard::getScores() const {
    return this -> scores;
}

CardCollection::CardCollection()
    : cards(std::vector<std::unique_ptr<ScorableCard>>()) {}
const std::vector<std::unique_ptr<ScorableCard>>& CardCollection::getCards() {
    return this->cards;
}
void CardCollection::push(std::unique_ptr<ScorableCard> card) {
    this->cards.push_back(std::move(card));
}
std::optional<std::unique_ptr<ScorableCard>> CardCollection::pull() {
    if (this->cards.empty()) {
        return std::nullopt;
    } else {
        auto result = std::move(this->cards.back());
        this->cards.pop_back();
        return result;
    }
}
void CardCollection::shuffle() {
    std::shuffle(
        this->cards.begin(),
        this->cards.end(),
        std::mt19937(std::random_device()())
    );
}
int CardCollection::getCount() {
    return this -> cards.size();
}

std::vector<int> CardCollection::getScores() const {
    std::vector<int> total_scores = {0};
    for (const std::unique_ptr<ScorableCard>& card: this->cards) {
        std::vector<int> temp_scores = {};
        for (const int &card_score: card->getScores()) {
            std::vector<int> per_card_scores = {};
            for (const int& curr_score: total_scores) {
                per_card_scores.push_back(card_score + curr_score);
            }
            temp_scores.insert(temp_scores.end(), per_card_scores.begin(), per_card_scores.end());
        }
        total_scores = temp_scores;
    }
    return total_scores;
}
std::vector<int> CardCollection::getValidScores() const {
    std::vector<int> scores = this->getScores();
    std::vector<int> scores_vector;
    for (int score: scores) {
        if (score <= 21) {
            scores_vector.push_back(score);
        }
    }
    return scores_vector;
}
void CardCollection::clear() {
    this->cards.clear();
}

void generateStandardDeck(CardCollection &collection) {
    collection.clear();

    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Two));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Three));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Four));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Five));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Six));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Seven));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Eight));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Nine));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Ten));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Jack));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Queen));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::King));
    collection.push(std::make_unique<ScorableCard>(Suit::Clubs, Rank::Ace));

    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Two));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Three));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Four));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Five));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Six));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Seven));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Eight));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Nine));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Ten));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Jack));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Queen));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::King));
    collection.push(std::make_unique<ScorableCard>(Suit::Hearts, Rank::Ace));

    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Two));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Three));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Four));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Five));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Six));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Seven));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Eight));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Nine));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Ten));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Jack));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Queen));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::King));
    collection.push(std::make_unique<ScorableCard>(Suit::Diamonds, Rank::Ace));

    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Two));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Three));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Four));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Five));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Six));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Seven));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Eight));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Nine));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Ten));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Jack));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Queen));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::King));
    collection.push(std::make_unique<ScorableCard>(Suit::Spades, Rank::Ace));
}
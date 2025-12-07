#include "shoe.h"

Shoe::Shoe(int decks, float penetration, QObject* parent) :
    QObject{parent}, decks_(decks), penetration_(penetration),
    cards_(), rng_() {
    shuffle();
}

Card Shoe::draw() {
    Card result = cards_.last();
    cards_.removeLast();
    return result;
}

size_t Shoe::getSize() const {
    return cards_.size();
}

void Shoe::shuffle() {
    for (int i = 0; i < decks_; ++i)
        addDeck();
    randomize(cards_, rng_);
    cards_.insert(decks_ * penetration_, Card(Card::Rank::Cut, Card::Suit::Cut));
}

void Shoe::addDeck() {
    for (int suitInt = 0; suitInt < 4; ++suitInt) {
        for (int rankInt = 1; rankInt < 14; ++rankInt)
            cards_.append(Card(static_cast<Card::Rank>(rankInt),
                               static_cast<Card::Suit>(suitInt)));
    }
}

void Shoe::randomize(QVector<Card>& list, QRandomGenerator& rng) {
    std::shuffle(list.begin(), list.end(), rng);
}

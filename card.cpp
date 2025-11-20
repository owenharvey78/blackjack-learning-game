#include "card.h"

Card::Card(Rank rank, Suit suit, QObject *parent)
    : QObject{parent}, rank_(rank), suit_(suit)
{}

int Card::getValue() const {
    if (static_cast<int>(rank_) >= 2 && static_cast<int>(rank_) <= 6) {
        return 1;
    }
    else if (rank_ == Rank::Ace || static_cast<int>(rank_) >= 10) {
        return -1;
    }
    return 0;
}

#include "card.h"

Card::Card(Rank rank, Suit suit)
    : rank_(rank), suit_(suit)
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

bool Card::isAce() const  {
    return rank_ == Rank::Ace;
}

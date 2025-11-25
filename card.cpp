#include "card.h"
#include <algorithm>

Card::Card(Rank rank, Suit suit)
    : rank(rank), suit(suit)
{}

int Card::getBlackjackValue() const {
    return std::min(static_cast<int>(rank), 10);
}

int Card::getHiLoValue() const {
    if (static_cast<int>(rank) >= 2 && static_cast<int>(rank) <= 6) {
        return 1;
    }
    else if (rank == Rank::Ace || static_cast<int>(rank) >= 10) {
        return -1;
    }
    return 0;
}

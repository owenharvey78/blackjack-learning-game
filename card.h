#ifndef CARD_H
#define CARD_H

#endif // CARD_H

struct Card {
    enum class Suit { Hearts, Diamonds, Clubs, Spades };
    enum class Rank { Ace = 1, Two, Three, Four, Five, Six, Seven,
                      Eight, Nine, Ten, Jack, Queen, King };

    Suit suit;
    Rank rank;
};

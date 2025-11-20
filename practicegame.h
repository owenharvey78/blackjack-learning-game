#ifndef PRACTICEGAME_H
#define PRACTICEGAME_H

#include "blackjack_rules.h"

class PracticeGame
{
public:
    PracticeGame();





private:

    // Rules for the practice game
    BlackjackRules rules_;

    // Cards in my hand
    std::vector<Card> myHand_;

    // Dealers hand
    std::vector<Card> dealerHand_;

    // Current deck
    // Shoe gameDeck_
};

#endif // PRACTICEGAME_H

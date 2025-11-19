#ifndef BLACKJACKRULES_H
#define BLACKJACKRULES_H

#include "blackjack_rules.h"
#include <vector>
#include "card.h"
blackjack_rules::blackjack_rules() {}

class BlackjackRules {

public:
    /// @brief gets the total value of the hand.
    /// @return int representing hand value.
    /// @param hand vector holding the cards
    static int getHandValue(const std::vector<Card>& hand);

    /// @brief determines if the current hand is a
    /// @param hand vector holding the cards
    bool isBust(const std::vector<Card>& hand);

    /// @brief determines if the current hand is a
    /// @param hand vector holding the cards
    bool isBlackjack(const std::vector<Card>& hand);

    /// @brief determines if the current hand is a soft hand
    /// @param hand vector holding the cards
    static bool isSoftHand(const std::vector<Card>& hand);
}

#endif // BLACKJACKRULES_H

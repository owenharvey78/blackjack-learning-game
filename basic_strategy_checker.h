#ifndef BASIC_STRATEGY_CHECKER_H
#define BASIC_STRATEGY_CHECKER_H

/// @brief Holds the BlackJack basic strategy plays for H17 and S17, allowing one to
/// find the optimal play for a given hand and dealer upcard.
class BasicStrategyChecker
{
public:
    /// @brief Defines the possible actions a player can take for a hand.
    enum class PlayerAction {
        Hit,
        Stand,
        Double,
        Split,
        Surrendur
    };

    /// @brief Initializes a new BasicStrategyChecker.
    /// @param dealaerHitsSoft17 Indicates whether the dealer hits (true) or
    /// stands (false) on soft 17, based on the ruleset.
    BasicStrategyChecker(bool dealerHitsSoft17 = true);
};

#endif // BASIC_STRATEGY_CHECKER_H

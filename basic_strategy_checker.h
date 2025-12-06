#ifndef BASIC_STRATEGY_CHECKER_H
#define BASIC_STRATEGY_CHECKER_H

#include <QVector>
#include "card.h"

/// @brief Holds the BlackJack basic strategy plays for H17 and S17, allowing one to
/// find the optimal play for a given hand and dealer upcard.
class BasicStrategyChecker
{
public:
    /// @brief Defines the possible actions a player can take for a hand. One of the
    /// enumerations, SplitIfDas, is not a real action—this value indicates that the
    /// player should split only if double after split (DAS) is allowed under the
    /// current ruleset.
    enum class PlayerAction {
        Hit,
        Stand,
        Double,
        Split,
        SplitIfDas,
        Surrendur
    };

    /// @brief Initializes a new BasicStrategyChecker.
    /// @param dealaerHitsSoft17 Indicates whether the dealer hits (true) or
    /// stands (false) on soft 17, based on the ruleset.
    BasicStrategyChecker(bool dealerHitsSoft17 = true);

    /// @brief Gets the best move for the given player hand against the dealer's upcard.
    /// @param hand The list of cards in the player's hand.
    /// @param dealerUpcard The dealer's first drawn card (the upcard).
    /// @return The best move for the given hand against the dealer's upcard.
    PlayerAction getBestMove(const QVector<Card>& hand, Card dealerUpcard);

    /// @brief Gets the second-best move for the given player hand against the dealer's
    /// upcard. If there is no second-best move (i.e. the best move is always possible),
    /// the result is the same as getBestMove.
    /// @param hand The lsit of cards in the player's hand.
    /// @param dealerUpcard The dealer's first drawn card (the upcard).
    /// @return The second-best move for the given hand against the dealer's upcard, or
    /// the best move if no second-best move is necessary.
    PlayerAction getSecondBestMove(const QVector<Card>& hand, Card dealerUpcard);

private:
    /// @brief The optimal actions for any hand with a hard total in the H17 ruleset. The
    /// rows (first indices) represent the player's hard total (from 4 to 20, inclusive),
    /// while the columns represent the dealer's upcard (2-10, then ace; face cards count
    /// as 10).
    static constexpr PlayerAction H17_HARD_TOTALS[19][10] {
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand }
    };

    /// @brief The optimal actions for any hand with a hard total in the S17 ruleset. The
    /// rows (first indices) represent the player's hard total (from 4 to 20, inclusive),
    /// while the columns represent the dealer's upcard (2-10, then ace; face cards count
    /// as 10).
    static constexpr PlayerAction S17_HARD_TOTALS[19][10] {
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand }
    };
};

#endif // BASIC_STRATEGY_CHECKER_H

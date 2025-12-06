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
        Surrender
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

    /// @brief Gets the third-best move for the given player hand against the dealer's
    /// upcard. For almost every possible hand, the second-best move will always be
    /// possible, so this method will return the same result as getSecondBestMove. The
    /// only exception is [8, 8] against the dealer's ace in the H17 ruleset, in which
    /// case the best move is to surrender, the second-best is to split, and the third-
    /// best is to hit. Thus, the only case where this method's result is different
    /// from getSecondBestMove is in this scenario.
    /// @param hand The lsit of cards in the player's hand.
    /// @param dealerUpcard The dealer's first drawn card (the upcard).
    /// @return The second-best move for the given hand against the dealer's upcard, or
    /// the best move if no second-best move is necessary.
    PlayerAction getThirdBestMove(const QVector<Card>& hand, Card dealerUpcard);

private:
    /// @brief The optimal actions for any hand with a hard total in the H17 ruleset. The
    /// rows (first indices) represent the player's hard total (from 4 to 20, inclusive),
    /// while the columns represent the dealer's upcard (2-10, then ace; face cards count
    /// as 10).
    static constexpr PlayerAction H17_HARD_TOTALS[17][10] {
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
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Surrender,PlayerAction::Surrender },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Surrender,PlayerAction::Surrender,PlayerAction::Surrender },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Surrender },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand }
    };

    /// @brief The optimal actions for any hand with a soft total in the H17 ruleset. The
    /// rows (first indices) represent the player's soft total, from 12 (A, A) to 20 (A,
    /// 9) inclusive (soft 12 is included in case the player cannot split). The columns
    /// (second indices) represent the dealer's upcard (2-10, then ace; face cards count
    /// as 10).
    static constexpr PlayerAction H17_SOFT_TOTALS[9][10] {
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Double, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand }
    };

    /// @brief The optimal actions for any hand with two cards of the same rank in the H17
    /// ruleset. The rows (first indices) represent the rank of card (2-10, then ace; face
    /// cards count as 10), and the columns (second indices) represent the dealer's upcard
    /// (2-10, then ace, with face cards counting as 10.
    static constexpr PlayerAction H17_SPLITTING[10][10] {
        { PlayerAction::SplitIfDas, PlayerAction::SplitIfDas, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::SplitIfDas, PlayerAction::SplitIfDas, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::SplitIfDas, PlayerAction::SplitIfDas, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::SplitIfDas, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Surrender },
        { PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Stand, PlayerAction::Split, PlayerAction::Split, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Double, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split },
    };

    /// @brief The optimal actions for any hand with a hard total in the S17 ruleset. The
    /// rows (first indices) represent the player's hard total (from 4 to 20, inclusive),
    /// while the columns represent the dealer's upcard (2-10, then ace; face cards count
    /// as 10).
    static constexpr PlayerAction S17_HARD_TOTALS[17][10] {
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
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Surrender,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit,PlayerAction::Hit,PlayerAction::Surrender,PlayerAction::Surrender,PlayerAction::Surrender },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand }
    };

    /// @brief The optimal actions for any hand with a soft total in the S17 ruleset. The
    /// rows (first indices) represent the player's soft total, from 12 (A, A) to 20 (A,
    /// 9) inclusive (soft 12 is included in case the player cannot split). The columns
    /// (second indices) represent the dealer's upcard (2-10, then ace; face cards count
    /// as 10).
    static constexpr PlayerAction S17_SOFT_TOTALS[9][10] {
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Hit, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Hit, PlayerAction::Hit,PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand }
    };

    /// @brief The optimal actions for any hand with two cards of the same rank in the S17
    /// ruleset. The rows (first indices) represent the rank of card (2-10, then ace; face
    /// cards count as 10), and the columns (second indices) represent the dealer's upcard
    /// (2-10, then ace, with face cards counting as 10.
    ///
    /// S17 splitting strategy is the same as H17 except for [8, 8] against a dealer's ace,
    /// in which case the player should stand in S17 but surrender in H17.
    static constexpr PlayerAction S17_SPLITTING[10][10] {
        { PlayerAction::SplitIfDas, PlayerAction::SplitIfDas, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::SplitIfDas, PlayerAction::SplitIfDas, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::SplitIfDas, PlayerAction::SplitIfDas, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Double, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::SplitIfDas, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit, PlayerAction::Hit },
        { PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split },
        { PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Stand, PlayerAction::Split, PlayerAction::Split, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Double, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand, PlayerAction::Stand },
        { PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split, PlayerAction::Split },
    };

    /// @brief Indicates whether to use the H17 ruleset (true) or the S17 ruleset (false).
    bool dealerHitsSoft17_;

    /// @brief Gets the column index for any of the basic strategy tables based on the
    /// dealer's upcard.
    /// @param upcardValue The value of the dealer's upcard, as defined by the
    /// getBlackjackValue method of the Card struct.
    /// @return A valid column index (0-9, inclusive) for any of the basic strategy tables
    /// based on the rank of the dealer's upcard.
    static inline int getUpcardIndex(int upcardValue);

    /// @brief Gets the row index (first index) for the H17/S17 hard totals table based on
    /// the hand total.
    /// @param handTotal The hard total of the hand (4-20, inclusive).
    /// @return The row index for the hard totals table (0-18, inclusive).
    static inline int getHardTotalsRowIndex(int handTotal);

    /// @brief Gets the row index (first index) for either of the soft totals tables based
    /// on the soft total of the hand (the total if counting ace as 11).
    /// @param softTotal The total value of the hand if counting ace as 11 (12-20, inclusive).
    /// @return A valid row index for the soft totals table (0-8, inclusive).
    static inline int getSoftTotalsRowIndex(int softTotal);

    /// @brief Gets the row index (first index) for either of the splitting tables based
    /// on the value of the two cards in the player's hand.
    /// @param blackjackValue The value of the two cards in the player's hand, as defined
    /// by the getBlackjackValue method of the Card struct.
    /// @return A valid row index for the splitting table (0-9, inclusive).
    static inline int getSplittingRowIndex(int blackjackValue);
};

#endif // BASIC_STRATEGY_CHECKER_H

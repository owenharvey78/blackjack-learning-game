#ifndef BLACKJACK_GAME_H
#define BLACKJACK_GAME_H

#include "card.h"
#include "ruleset.h"
#include "shoe.h"
#include <QObject>

class BlackJackGame : public QObject {
    Q_OBJECT

public:

    /// @brief
    explicit BlackJackGame(QObject *parent = nullptr);

    /// @brief defines an enum for the different possible results of a hand.
    enum class GameResult {
        PlayerWin,
        PlayerBlackjack,
        DealerWin,
        Push,
        PlayerBust,
        DealerBust
    };

    // Utility methods

    /// @brief Change the ruleset of the game.
    /// @param rules The new ruleset.
    void setRuleset(Ruleset rules);

public slots:
    /// @brief Starts the next round/deal.
    void dealNewHand();

    /// @brief Player hits to draw another card.
    void playerHit();

    /// @brief Player stands to not draw another card.
    void playerStand();

    /// @brief Player splits bet and hand.
    void playerSplit();

    /// @brief Player doubles bet and recieves one more card and can no longer hit.
    void playerDoubleDown();

    /// @brief Checks game state.
    void checkCardsAndRound(int handIndex, GameResult currentState);

    /// @brief Starts the round.
    void startRound();

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======

=======
>>>>>>> parent of 8393471 (Revert "Add signals to blackjack game logic")
=======
>>>>>>> 79c7c7bbc97254e6319ff7d5f20bb3c0afb10bc1
private slots:
    /// @brief Recursive helper for dealer's turn.
    void continueDealerTurn();

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 911dab15d53891492122a98eb6959582bae77684
=======
>>>>>>> parent of 8393471 (Revert "Add signals to blackjack game logic")
=======
>>>>>>> 79c7c7bbc97254e6319ff7d5f20bb3c0afb10bc1
private:
    /// @brief Dealer hits to draw another card.
    void dealerHit();

    /// @brief Dealer stands, round/deal ends.
    void dealerStand();

    /// @brief Helper to draw card from shoe.
    Card drawCardFromShoe();

    /// @brief Emits that the dealer's turn has started.
    void dealerTurn();

    // Static helper methods.

    /// @brief gets the total value of the hand.
    /// Handles logic of ace being 1 or 11.
    /// @param hand vector holding the cards.
    /// @return reports aces as 11 unless that results in a bust.
    static int getHandValue(QVector<Card>& hand);

    /// @brief determines if the current hand is a.
    /// @param hand vector holding the cards.
    /// @param hand vector holding the cards.
    /// @return true if the hand is a bust
    static bool isBust(QVector<Card>& hand);

    /// @brief determines if the current hand is a blackjack.
    /// @param hand vector holding the cards.
    /// @return true if is blackjack.
    static bool isBlackJack(const QVector<Card>& hand);

    /// @brief determines if the current hand is 21.
    /// @param hand vector holding the cards.
    /// @return true if is 21.
    static bool is21(const QVector<Card>& hand);

    /// @brief determines if the current hand is contains an ace.
    /// @param hand vector holding the cards.
    /// @return true if is soft hand.
    static bool isSoftHand(const QVector<Card>& hand);

    /// @brief determines the winner of the hand.
    /// @param playerHand vector holding player's hand.
    /// @param dealerHand vector holding dealer's hand.
    /// @return game result.
    static GameResult determineWinner(QVector<Card>& playerHand, QVector<Card>& dealerHand);

    // Non-static game state methods.

    /// @brief determines if the dealer should hit.
    /// @param hand vector holding the cards.
    /// @return true if dealer should hit.
    bool dealerShouldHit(QVector<Card>& hand) const;

    /// @brief determines if the player can double.
    /// @param hand vector holding the cards.
    /// @param currentSplitCount The number of times the player has split this round.
    /// @return true if player can double.
    bool canDouble(const QVector<Card>& hand, int currentSplitCount) const;

    /// @brief determines if the player can surrender.
    /// @param hand vector holding the cards.
    /// @return true if player can surrender.
    bool canSurrender(const QVector<Card>& hand) const;

    /// @brief determines if the player can split.
    /// @param hand vector holding the cards.
    /// @param currentSplitCount amount of times already split.
    /// @return true if player can split.
    bool canSplit(const QVector<Card>& hand, int currentSplitCount) const;

    // Member variables.

    /// @brief Holds the ruleset.
    Ruleset rules_;

    /// @brief The shoe of the blackjack game.
    Shoe* shoe_;

    /// @brief Holds the player's current hand.
    QVector<QVector<Card>> playerHands_;

    /// @brief Holds the dealer's current hand.
    QVector<Card> dealerHand_;

    /// @brief Holds the total chip amount.
    int totalChipAmount;

    /// @brief True if the shoe needs shuffling. False otherwise.
    bool needsShuffling_;

    /// @brief True if the current round/deal has started. False otherwise.
    bool hasRoundStarted_;

    /// @brief Tracks which hand is currently active to account for split hands.
    int currentHandIndex_;

signals:
    void startNewHand();

    void shuffleCards();

    void dealPlayerCard(int handIndex, Card card);

    void dealDealerCard(Card card);

    void roundOver(int handIndex, GameResult result);

    void splitHand(int handCount);

    void turnChanged(bool isPlayerTurn, int handIndex);
};

#endif // BLACKJACK_GAME_H

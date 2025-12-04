#ifndef BLACKJACK_GAME_H
#define BLACKJACK_GAME_H

#include "card.h"
#include "ruleset.h"
#include "shoe.h"
#include <QObject>
#include <QTimer>

class BlackjackGame : public QObject {
    Q_OBJECT

public:

    /// @brief
    explicit BlackjackGame(QObject *parent = nullptr);

    /// @brief defines an enum for the different possible results of a hand.
    enum class GameResult {
        Win,
        Lose,
        Push,
        Blackjack
    };

    /// @brief Change the ruleset of the game.
    /// @param rules The new ruleset.
    void setRuleset(Ruleset rules);

public slots:
    /// @brief Signals that the player's balance has changed, then starts a new
    /// round.
    void beginRound(int betAmount);

    /// @brief Player hits to draw another card.
    void playerHit();

    /// @brief Player stands to not draw another card.
    void playerStand();

    /// @brief Player splits bet and hand.
    void playerSplit();

    /// @brief The player doubles their current bet and draws only one more card.
    void playerDouble();

    /// @brief Gets the running count for the game.
    int getRunningCount();

    /// @brief Gets the true count for the game.
    int getTrueCount();

signals:
    // Signals used by game widget.

    /// @brief Emitted when a card is dealt to the player (for animation).
    /// @param card The card that is dealt.
    /// @param handIndex The hand that is dealt to the player.
    /// @param isLastCard Indicates whether this is the last card to be dealt
    /// for this hand (if the player doubled or split aces) and should be
    /// dealt sideways.
    void playerCardDealt(Card card, int handIndex, bool isLastCard);

    /// @brief Emitted when a card is dealt to the dealer (for animation).
    void dealerCardDealt(Card card);

    /// @brief Emitted when the round is finished.
    /// @param result The result of the round.
    /// @param payout The amount the player should be paid (including their original bet).
    /// @param handIndex Which hand this result is for (0-based index).
    /// @param totalHands Total number of hands in this round.
    void roundEnded(BlackjackGame::GameResult result, int payout, int handIndex, int totalHands);

    // Internal logic signals.

    /// @brief Emitted when a hand splits (to update UI hand count).
    /// @param handIndex The index of the hand that will be split.
    void splitHand(int handIndex);

    /// @brief Emitted when it becomes the player's turn.
    /// @param handIndex Index of the active hand.
    /// @param canDouble True if player can double on current hand.
    /// @param canSplit True if player can split current hand.
    void playerTurn(int handIndex, bool canDouble, bool canSplit);

    /// @brief Emitted when it becomes the dealer's turn.
    void dealerTurnStarted();

    /// @brief Signals that the player has placed a bet (either during the
    /// betting stage, by doubling, or by splitting).
    /// @param amount The total amount of the bet the player has placed.
    void betPlaced(int amount);

private slots:
    /// @brief Recursive helper for dealer's turn (called by QTimer).
    void continueDealerTurn();

    /// @brief Processes the next hand's result with appropriate delay.
    void processNextHandResult();

private:
    // Internal helper methods.
    /// @brief Resets state and calls dealCards().
    void dealNewHand();

    /// @brief Handles the initial deal animation with timers.
    void dealCards();

    /// @brief Deals a card to the dealer.
    void dealDealerCard();

    /// @brief Deals a card to the player.
    /// @param handIndex The index of the hand to deal to.
    /// @param isLastDeal Indicates whether the card to be dealt will be the
    /// last card dealt for the hand (i.e. if the player doubled or split aces)
    /// and should be dealt sideways.
    void dealPlayerCard(int handIndex, bool isLastDeal);

    /// @brief Helper to resolve the round and emit roundEnded.
    void checkCardsAndRound(int handIndex, GameResult currentState);

    /// @brief Helper to draw card from shoe.
    Card drawCardFromShoe();

    /// @brief determines the winner of the hand.
    /// @param playerHand vector holding player's hand.
    /// @param dealerHand vector holding dealer's hand.
    /// @return game result.
    static GameResult determineWinner(QVector<Card>& playerHand, QVector<Card>& dealerHand);

    /// @brief Inidicates dealer turn.
    void dealerTurn();

    /// @brief Determines if the player can double based on their current active
    /// hand and the ruleset.
    /// @return true if player can double, false otherwise.
    bool canDouble() const;

    /// @brief determines if the player can surrender.
    /// @param hand vector holding the cards.
    /// @return true if player can surrender.
    bool canSurrender() const;

    /// @brief determines if the player can split.
    /// @return true if player can split.
    bool canSplit() const;

    // Dealer methods
    /// @brief Dealer hits to draw another card.
    void dealerHit();

    /// @brief Dealer stands, round/deal ends.
    void dealerStand();

    /// @brief determines if the dealer should hit.
    /// @param hand vector holding the cards.
    /// @return true if dealer should hit.
    bool dealerShouldHit(QVector<Card>& hand) const;

    /// @brief Checks if all player hands are busted.
    /// @return true if all hands are busted, false otherwise.
    bool allHandsBusted() const;

    // Static game state methods.
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

    // Member variables.
    /// @brief Holds the ruleset.
    Ruleset rules_;

    /// @brief The shoe of the blackjack game.
    Shoe* shoe_;

    /// @brief The amount of money the player has.
    int balance_;

    /// @brief The current bet amount for the round.
    int currentBetAmount_;

    /// @brief Holds the player's current hands.
    QVector<QVector<Card>> playerHands_;

    /// @brief Holds the bet amounts for each of the player's hands.
    QVector<int> betAmounts_;

    /// @brief Holds the dealer's current hand.
    QVector<Card> dealerHand_;

    /// @brief True if the shoe needs shuffling. False otherwise.
    bool needsShuffling_;

    /// @brief True if the current round/deal has started. False otherwise.
    bool hasRoundStarted_;

    /// @brief Tracks which hand is currently active to account for split hands.
    int currentHandIndex_;

    /// @brief Index of hand currently being processed for result display.
    int resultHandIndex_;

    /// @brief Holds the running count of all the cards dealt
    int runningCount_;

};

#endif // BLACKJACK_GAME_H

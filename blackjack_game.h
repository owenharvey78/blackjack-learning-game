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

private slots:

    /// @brief Starts the game.
    void gameStart();

    /// @brief Starts the next round/deal.
    void nextDeal();

    /// @brief Deals cards to player and dealer.
    void dealCards();

    /// @brief Player hits to draw another card.
    void playerHit();

    /// @brief Player stands to not draw another card.
    void playerStand();

    /// @brief Player splits bet and hand.
    void playerSplit();

    /// @brief Checks game state.
    void checkCardsAndRound(GameResult currentState);

private:

    // Game Logic Methods

    /// @brief Dealer hits to draw another card.
    void dealerHit();

    /// @brief Dealer stands, round/deal ends.
    void dealerStand();

    // Game state methods.

    /// @brief gets the total value of the hand.
    /// Handles logic of ace being 1 or 11.
    /// @param hand vector holding the cards.
    /// @return reports aces as 11 unless that results in a bust.
    int getHandValue(std::vector<Card>& hand) const;

    /// @brief determines if the current hand is a.
    /// @param hand vector holding the cards.
    /// @param hand vector holding the cards.
    /// @return true if the hand is a bust
    bool isBust(std::vector<Card>& hand) const;

    /// @brief determines if the current hand is a blackjack.
    /// @param hand vector holding the cards.
    /// @return true if is blackjack.
    bool isBlackjack(const std::vector<Card>& hand) const;

    /// @brief determines if the current hand is 21.
    /// @param hand vector holding the cards.
    /// @return true if is 21.
    bool is21(const std::vector<Card>& hand) const;

    /// @brief determines if the current hand is contains an ace.
    /// @param hand vector holding the cards.
    /// @return true if is soft hand.
    bool isSoftHand(const std::vector<Card>& hand) const;

    /// @brief determines the winner of the hand.
    /// @param playerHand vector holding player's hand.
    /// @param dealerHand vector holding dealer's hand.
    /// @return game result.
    GameResult determineWinner(std::vector<Card>& playerHand, std::vector<Card>& dealerHand) const;

    /// @brief determines if the dealer should hit.
    /// @param hand vector holding the cards.
    /// @return true if dealer should hit.
    bool dealerShouldHit(std::vector<Card>& hand) const;

    /// @brief determines if the player can double.
    /// @param hand vector holding the cards.
    /// @return true if player can double.
    bool canDouble(const std::vector<Card>& hand) const;

    /// @brief determines if the player can surrender.
    /// @param hand vector holding the cards.
    /// @return true if player can surrender.
    bool canSurrender(const std::vector<Card>& hand) const;

    /// @brief determines if the player can split.
    /// @param hand vector holding the cards.
    /// @param currentSplitCount amount of times already split.
    /// @return true if player can split.
    bool canSplit(const std::vector<Card>& hand, int currentSplitCount) const;

    // Member variables.

    /// @brief Holds the ruleset.
    Ruleset rules_;

    /// @brief The shoe of the blackjack game.
    Shoe* shoe_;

    /// @brief Holds the player's current hand.
    std::vector<Card> playerHand_;

    /// @brief Holds the dealer's current hand.
    std::vector<Card> dealerHand_;

    /// @brief Holds the total chip amount.
    int totalChipAmount;

    /// @brief True if the shoe needs shuffling. False otherwise.
    bool needsShuffling_;

    /// @brief True if the current round/deal has started. False otherwise.
    bool hasRoundStarted_;

signals:

};

#endif // BLACKJACK_GAME_H

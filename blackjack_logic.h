#ifndef BLACKJACK_LOGIC_H
#define BLACKJACK_LOGIC_H

#include "card.h"
#include "ruleset.h"
#include <QObject>

class BlackJackLogic : public QObject {
    Q_OBJECT

public:

    /// @brief
    explicit BlackJackLogic(QObject *parent = nullptr);

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

    /// @brief change the ruleset of the game.
    /// @param rules new ruleset.
    void updateRules(Ruleset rules);

private slots:

    /// @brief
    void gameStart();

    /// @brief
    void nextDeal();

    /// @brief
    void playerHit();

    /// @brief
    void playerStand();

    /// @brief
    void playerSplit();

private:

    // Game Logic Methods

    /// @brief
    void dealerHit();

    /// @brief
    void dealerStand();

    // Game state methods.

    /// @brief gets the total value of the hand.
    /// Handles logic of ace being 1 or 11.
    /// @param hand vector holding the cards.
    /// @return reports aces as 11 unless that results in a bust.
    int getHandValue(std::vector<Card>& hand);

    /// @brief determines if the current hand is a.
    /// @param hand vector holding the cards.
    /// @param hand vector holding the cards.
    /// @return true if the hand is a bust
    bool isBust(std::vector<Card>& hand);

    /// @brief determines if the current hand is a blackjack.
    /// @param hand vector holding the cards.
    /// @return true if is blackjack.
    bool isBlackjack(const std::vector<Card>& hand);

    /// @brief determines if the current hand is contains an ace.
    /// @param hand vector holding the cards.
    /// @return true if is soft hand.
    bool isSoftHand(const std::vector<Card>& hand);

    /// @brief determines the winner of the hand.
    /// @param playerHand vector holding player's hand.
    /// @param dealerHand vector holding dealer's hand.
    /// @return game result.
    GameResult determineWinner(std::vector<Card>& playerHand, std::vector<Card>& dealerHand) const;

    /// @brief determines if the dealer should hit.
    /// @param hand vector holding the cards.
    /// @return true if dealer should hit.
    bool shouldDealerHit(std::vector<Card>& hand) const;

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


    /// @brief hold the ruleset.
    Ruleset rules_;

    /// @brief
    // Shoe* shoe_;

    /// @brief Holds the player's current hand.
    std::vector<Card>* playerHand_;

    /// @brief Holds the dealer's current hand.
    std::vector<Card>* dealerHand_;

signals:

};

#endif // BLACKJACK_LOGIC_H

#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVariantAnimation>
#include "blackjack_game.h"
#include "card.h"
#include "card_sprites.h"

namespace Ui {
class GameWidget;
}

/// @brief The widget defining the view for the main blackjack gameplay.
class GameWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructs a new GameWidget.
    /// @param game The game to be played.
    /// @param parent The parent widget of this widget.
    explicit GameWidget(BlackjackGame* game, QWidget* parent = nullptr);

    /// @brief Frees any resources associated with this GameWidget.
    ~GameWidget();

    /// @brief Triggers the betting stage, displaying the buttons for the user
    /// to place their bet and start the round.
    void beginBetStage();

signals:
    /// @brief Signals that the player has completed betting and begun the round.
    /// @param betAmount The bet the player placed.
    void beginRound(int betAmount);

protected:
    /// @brief Handles window/widget resize events to maintain aspect ratio.
    void resizeEvent(QResizeEvent* event) override;

public slots:
    /// @brief Updates the balance label when a bet is placed (either during betting,
    /// by doubling, or by splitting). Shows the amount that was bet underneath the
    /// balance for two seconds, then updates the balance label and hides the bet
    /// amount.
    /// @param betAmount The amount the player bet.
    /// @param newBalance The player's balance after making the bet.
    void onBetPlaced(int betAmount, int newBalance);

private slots:

    /// @brief During the betting stage, adds a chip of the given value to the
    /// current bet.
    void addChip(int value);

    /// @brief During the betting stage, removes a chip of the given value from
    /// the current bet.
    void removeChip(int value);

    /// @brief Starts the game, enables displays.
    void onStartButtonClicked();

    /// @brief Animates a player card being dealt.
    /// @param card The card to deal.
    /// @param handIndex The index of the hand to deal to.
    /// @param isLastCard Indicates whether the card should be dealt sideways.
    void onPlayerCardDealt(Card card, int handIndex, bool isLastCard);

    /// @brief Animates a dealer card being dealt.
    /// @param card The card to deal.
    void onDealerCardDealt(Card card);

    /// @brief Recieves the game result from game logic class.
    /// @param result of the game.
    void onRoundEnded(BlackjackGame::GameResult result);

    /// @brief Handles player's turn starting.
    /// @param handIndex Index of the active hand.
    /// @param canDouble True if player can double on current hand.
    /// @param canSplit True if player can split current hand.
    void onPlayerTurn(int handIndex, bool canDouble, bool canSplit);

    /// @brief Handles dealer's turn starting.
    void onDealerTurnStarted();

private:
    /// @brief The UI form associated with this widget.
    Ui::GameWidget* ui_;

    /// @brief The graphics/animation scene this widget holds.
    QGraphicsScene* scene_;

    /// @brief The view of the animation scene.
    QGraphicsView* view_;

    /// @brief The deck, unmoving (TBD).
    QGraphicsPixmapItem* deckItem_;

    /// @brief The deck position.
    QPoint deckPos_;

    /// @brief The game currently being played.
    BlackjackGame* game_;

    /// @brief The card as a class for easy access.
    CardSprites cardSprites_;

    /// @brief Timer for delaying animations and such.
    QTimer timer_;

    /// @brief The player's balance (necessary to ensure the player doesn't
    /// bet more than they have).
    int balance_;

    /// @brief The cards the dealer currently has.
    QVector<Card> dealerHand_;

    /// @brief The cards the player currently has.
    QVector<QVector<Card>> playerHands_;

    /// @brief Keeps track of the cards drawn for animation. (Player)
    int playerHandIndex_;

    /// @brief Keeps track of the cards drawn for animation. (Dealer)
    int dealerHandIndex_;

    /// @brief Keeps track of the currently active hand during play.
    int currentHandIndex_;

    /// @brief During the betting stage, represents the player's current bet.
    /// The keys represent the chip values (1, 5, 10, 25, 50, and 100). The
    /// values represent the number of chips the player has bet for each of
    /// those values.
    QMap<int, int> currentBet_;

    /// @brief During the betting stage, represents the total of the chips the
    /// player has bet.
    int currentBetTotal_;

    /// @brief Determines whether the chip buttons should be enabled based on the
    /// player's balance and current bet to prevent the player from betting more
    /// than they have.
    void setChipButtonsEnabled();

    /// @brief Helper for flipping a card.
    /// @param card The card to flip.
    void flipCard(QGraphicsPixmapItem* item, const Card& card);

    /// @brief Updates the view scaling to maintain aspect ratio.
    void updateViewScale();

    /// @brief Resets the game.
    void resetGame();

    /// @brief The graphics item for the dealer's hole card that will be flipped
    /// after the player is done.
    QGraphicsPixmapItem* holeCardItem_;

    /// @brief The dealer's hole card that will be flipped after the player is done.
    Card holeCard_;
};

#endif // GAME_WIDGET_H

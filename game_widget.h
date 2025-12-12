#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVariantAnimation>
#include <QMessageBox>
#include "blackjack_game.h"
#include "card.h"
#include "strategy_chart_dialog.h"
#include "cards_view.h"

namespace Ui {
class GameWidget;
}

/// @brief The widget defining the view for the main blackjack gameplay.
class GameWidget : public QWidget {
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

    /// @brief Signals to return to main menu.
    void returnToMainMenu();

protected:
    /// @brief Handles window/widget resize events to maintain aspect ratio.
    void resizeEvent(QResizeEvent* event) override;

public slots:
    /// @brief Updates the balance label when a bet is placed (either during betting,
    /// by doubling, or by splitting). Shows the amount that was bet underneath the
    /// balance for two seconds, then updates the balance label and hides the bet
    /// amount.
    /// @param betAmount The amount the player bet.
    void onBetPlaced(int betAmount);

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
    /// @param payout The amount paid for this hand.
    /// @param handIndex Which hand this result is for.
    /// @param totalHands Total number of hands in this round.
    void onRoundEnded(BlackjackGame::GameResult result, int payout, int handIndex, int totalHands);

    /// @brief Handles player's turn starting.
    /// @param handIndex Index of the active hand.
    /// @param canDouble True if player can double on current hand.
    /// @param canSplit True if player can split current hand.
    /// @param canSurrender True if the player can surrender.
    void onPlayerTurn(int handIndex, bool canDouble, bool canSplit, bool canSurrender);

    /// @brief Handles dealer's turn starting.
    void onDealerTurnStarted();

    /// @brief Handles hand split event for future multi-hand UI.
    /// @param handIndex The index of the hand being split.
    void onHandSplit(int handIndex);

    /// @brief Handles when the user wants to return to the main menu.
    void onReturnToMainMenu();

    /// @brief Displays the strategy chart for the current ruleset.
    void onStrategyChartButtonClicked();

    /// @brief Toggles whether the counting label is currently displayed.
    void toggleCountingLabel();

    /// @brief Shows the amount of decks still left in shoe.
    void showAmountOfDecksLeft();


private:
    /// @brief Updates the counting label with the new running and true count, resizing
    /// as necessary.
    void updateCountingLabel();

    /// @brief Converts a PlayerAction enum to a readable string for display
    /// @param action The action to convert
    /// @return String representation like "Hit", "Stand", "Double Down", etc.
    QString actionToString(BasicStrategyChecker::PlayerAction action) const;

    /// @brief Generates an educational explanation for why a move is recommended
    /// @param recommendedAction The action basic strategy recommends
    /// @param hand The player's hand
    /// @param dealerUpcard The dealer's upcard value
    /// @return Explanation string describing the recommended action for the specific situation
    QString generateStrategyExplanation(BasicStrategyChecker::PlayerAction recommendedAction,
                                       const QVector<Card>& hand, int dealerUpcard) const;

    /// @brief Checks if the player's action matches basic strategy and shows warning if not
    /// @param chosenAction The action the player chose to take
    /// @return True if should proceed (always true, but shows warning first if wrong)
    bool checkBasicStrategy(BasicStrategyChecker::PlayerAction chosenAction);

    /// @brief Updates the player's balance by udpateAmount. Then, displays
    /// text showing the change to the player's balance for updateDelay
    /// milliseconds; then animates the balance changing by that amount for
    /// animationDuration milliseconds.
    /// @param updateAmount The amount to be added to the balance.
    /// @param updateDelay The time to wait before animating the balance change.
    /// @param animationDuration The length of the balance update animation.
    void updateBalance(int updateAmount, int updateDelay, int animationDuration);

    /// @brief The size of the chip icons.
    static constexpr int CHIP_ICON_SIZE = 128;

    /// @brief The size of the buttons in the top-right corner (exit, strategy
    /// chart, and count).
    static constexpr int EXIT_ICON_SIZE = 64;

    /// @brief The size of the gameplay action buttons (hit, stand, double, split).
    static constexpr int GAMEPLAY_BUTTON_SIZE = 100;

    /// @brief The height of the "start round" button. The width of the button is fixed
    /// at 4x the height.
    static constexpr int START_BUTTON_HEIGHT = 100;

    /// @brief The duration between flashes of the start round button, in milliseconds.
    static constexpr int START_BUTTON_FLASH_DURATION = 500;

    /// @brief The UI form associated with this widget.
    Ui::GameWidget* ui_;

    /// @brief The game currently being played.
    BlackjackGame* game_;

    /// @brief Timer for delaying animations and such.
    QTimer timer_;

    /// @brief The player's balance (necessary to ensure the player doesn't
    /// bet more than they have).
    int balance_;

    /// @brief The cards the dealer currently has.
    QVector<Card> dealerHand_;

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

    /// @brief Resets the game.
    void resetGame();

    /// @brief The strategy chart overlay widget.
    StrategyChartDialog* strategyOverlay_;

    /// @brief The label showing the count.
    QLabel* countLabel_;

    /// @brief The label showing the round result.
    QLabel* resultLabel_;

    /// @brief The graphics view for displaying the cards.
    CardsView* cardsView_;

    /// @brief Timer for controlling the flashing animation of the start round button.
    QTimer* flashAnimationTimer_;

    /// @brief First icon for start round button animation.
    QIcon startRoundIconOne_;

    /// @brief Second icon for start round button animation.
    QIcon startRoundIconTwo_;

    /// @brief Whether the start round button is currently on the first (true) or second
    /// (false) frame of the animation.
    bool startRoundOnIconOne_;

    /// @brief Whether the count label is currently being displayed.
    bool showingCountLabel_;
};

#endif // GAME_WIDGET_H

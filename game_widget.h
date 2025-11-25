#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>
#include <QVector>
#include "blackjack_game.h"
#include "card.h"

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
    explicit GameWidget(BlackjackGame game, QWidget* parent = nullptr);

    /// @brief Frees any resources associated with this GameWidget.
    ~GameWidget();

private slots:
    /// @brief Triggers the betting stage, displaying the buttons for the user
    /// to place their bet and start the round.
    void beginBetStage();

    /// @brief During the betting stage, adds a chip of the given value to the
    /// current bet.
    void addChip(int value);

private:
    /// @brief The UI form associated with this widget.
    Ui::GameWidget* ui_;

    /// @brief The game currently being played.
    BlackjackGame game_;

    /// @brief The cards the dealer currently has.
    QVector<Card> dealerHand_;

    /// @brief The cards the player currently has.
    QVector<QVector<Card>> playerHands_;

    /// @brief During the betting stage, this represents the number of $1-valued
    /// chips the player has placed on the table.
    int current1Chips_;

    /// @brief During the betting stage, this represents the number of $5-valued
    /// chips the player has placed on the table.
    int current5Chips_;

    /// @brief During the betting stage, this represents the number of $10-valued
    /// chips the player has placed on the table.
    int current10Chips_;

    /// @brief During the betting stage, this represents the number of $25-valued
    /// chips the player has placed on the table.
    int current25Chips_;

    /// @brief During the betting stage, this represents the number of $50-valued
    /// chips the player has placed on the table.
    int current50Chips_;

    /// @brief During the betting stage, this represents the number of $100-valued
    /// chips the player has placed on the table.
    int current100Chips_;
};

#endif // GAME_WIDGET_H

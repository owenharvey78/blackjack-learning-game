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

public slots:
    /// @brief Displays the card that was dealt to the dealer.
    void dealerCardDealt(Card card);

private:
    /// @brief The UI form associated with this widget.
    Ui::GameWidget* ui_;

    /// @brief The game currently being played.
    BlackjackGame game_;

    /// @brief The cards the dealer currently has.
    QVector<Card> dealerHand_;

    /// @brief The cards the player currently has.
    QVector<QVector<Card>> playerHands_;

    /// @brief During the betting stage, this represents the bet the player will make
    /// once they click the "start" button.
    int betToPlace_;
};

#endif // GAME_WIDGET_H

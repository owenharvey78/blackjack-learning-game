#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>
#include "blackjack_game.h"

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

private:
    /// @brief The UI form associated with this widget.
    Ui::GameWidget* ui_;

    /// @brief The game currently being played.
    BlackjackGame game_;
};

#endif // GAME_WIDGET_H

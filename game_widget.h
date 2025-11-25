#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>

namespace Ui {
class GameWidget;
}

/// @brief The widget defining the view for the main blackjack gameplay.
class GameWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructs a new GameWidget.
    /// @param parent The parent widget of this widget.
    explicit GameWidget(QWidget* parent = nullptr);

    /// @brief Frees any resources associated with this GameWidget.
    ~GameWidget();

private:
    Ui::GameWidget* ui;
};

#endif // GAME_WIDGET_H

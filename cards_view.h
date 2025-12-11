#ifndef CARDS_VIEW_H
#define CARDS_VIEW_H

#include <QWidget>

/// @brief Displays the cards for a Blackjack game.
class CardsView : public QWidget
{
    Q_OBJECT
public:
    /// @brief Creates a new CardsView object.
    /// @param parent The parent widget of this CardsView
    explicit CardsView(QWidget* parent = nullptr);

signals:
};

#endif // CARDS_VIEW_H

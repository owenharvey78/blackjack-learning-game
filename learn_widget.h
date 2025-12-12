#ifndef LEARN_WIDGET_H
#define LEARN_WIDGET_H

#include <QWidget>
#include <QStringList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "card_sprites.h"
#include "card.h"
#include "blackjack_game.h"

namespace Ui {

class LearnWidget;
}

/// @brief This class represents the Learn page of our Blackjack education app
class LearnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LearnWidget(QWidget *parent = nullptr);
    ~LearnWidget();

private slots:

    // onNext and on Prevf handle the user moving through the slides.
    void onNextButtonClicked();
    void onPrevButtonClicked();

    /// @brief Handles the user input on page 12.
    void onCheckButtonClicked();

    /// @brief takes the user back to the main menu.
    void onMainMenuClicked();

    /// @brief called when the user presses the deal button on page 14.
    void startPracticeHand();

    // handles the simple blackjack game buttons on page 14.
    void onPracticeHitClicked();
    void onPracticeStandClicked();

    /// @brief sets up the cards for page 14
    void drawPracticeHand();

signals:

    /// @brief signals the mainwindow that we pressed the main menu.
    void returnToMainMenu();

private:

    Ui::LearnWidget *ui_;

    /// @brief stores the instruction strings in a String List
    QStringList instructionList_;   // QStringList provides more functionality to stings

    /// @brief stores the header strings in a String List
    QStringList headerList_;

    /// @brief Graphics scene for the learn page.
    QGraphicsScene* scene_;

    /// @brief keeps track of where we are in the instructions/header
    int currentInstruction_;

    CardSprites cardSprites_;

    /// @brief sets up the learning page and enables and disables buttons as needed
    void updatePage();

    /// @brief sets up the count cards on page 12
    void showCountingExampleCards();

    /// @brief Simple practice simulator in learn page.
    BlackjackGame* practiceGame_;

    /// @brief Vector holding the players hand
    QVector<Card> practiceHand_;

    /// @brief dealers up card.
    Card practiceDealerUpcard_;

};

#endif // LEARN_WIDGET_H

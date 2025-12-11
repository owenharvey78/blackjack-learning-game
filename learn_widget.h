#ifndef LEARN_WIDGET_H
#define LEARN_WIDGET_H

#include <QWidget>
#include <QStringList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "card_sprites.h"
#include "card.h"

namespace Ui {
class LearnWidget;
}

class LearnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LearnWidget(QWidget *parent = nullptr);
    ~LearnWidget();

private slots:

    void onNextButtonClicked();
    void onPrevButtonClicked();
    void onCheckButtonClicked();
    void onMainMenuClicked();
    void showCountingExampleCards();

signals:
    void returnToMainMenu();

private:
    Ui::LearnWidget *ui_;

    /// @brief stores the instruction strings in a String List
    QStringList instructionList_;   // QStringList provides more functionality to stings

    /// @brief stores the header strings in a String List
    QStringList headerList_;

    QGraphicsScene* scene_;

    CardSprites cardSprites_;

    /// @brief keeps track of where we are in the instructions/header
    int currentInstruction_;

    /// @brief sets up the learning page and enables and disables buttons as needed
    void updatePage();

};

#endif // LEARN_WIDGET_H

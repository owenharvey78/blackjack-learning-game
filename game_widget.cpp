#include "game_widget.h"
#include "ui_game_widget.h"

GameWidget::GameWidget(BlackjackGame* game, QWidget *parent)
    : QWidget(parent),
    ui_(new Ui::GameWidget),
    game_(game)
{
    ui_->setupUi(this);

    // Set up current bet map
    currentBet_[1] = 0;
    currentBet_[5] = 0;
    currentBet_[10] = 0;
    currentBet_[25] = 0;
    currentBet_[50] = 0;
    currentBet_[100] = 0;
}

GameWidget::~GameWidget()
{
    delete ui_;
}

void GameWidget::beginBetStage() {
    ui_->chip1Button->setVisible(true);
    ui_->chip5Button->setVisible(true);
    ui_->chip10Button->setVisible(true);
    ui_->chip25Button->setVisible(true);
    ui_->chip50Button->setVisible(true);
    ui_->chip100Button->setVisible(true);
    setChipButtonsEnabled();
    // TODO: hide other buttons if necessary
}

void GameWidget::addChip(int value) {
    currentBetTotal_ += value;
    currentBet_[value]++;
}

void GameWidget::setChipButtonsEnabled() {
    int newBalance = balance_ - currentBetTotal_;
    ui_->chip1Button->setEnabled(newBalance >= 1);
    ui_->chip5Button->setEnabled(newBalance >= 5);
    ui_->chip10Button->setEnabled(newBalance >= 10);
    ui_->chip25Button->setEnabled(newBalance >= 25);
    ui_->chip50Button->setEnabled(newBalance >= 50);
    ui_->chip100Button->setEnabled(newBalance >= 100);
}

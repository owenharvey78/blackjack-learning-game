#include "game_widget.h"
#include "ui_game_widget.h"

GameWidget::GameWidget(BlackjackGame* game, QWidget *parent)
    : QWidget(parent), ui_(new Ui::GameWidget), game_(game),
    cardSprites_(":/images/cards.png"), balance_(1000), playerHandIndex_(0),
    dealerHandIndex_(0), currentBetTotal_(0) {
    ui_->setupUi(this);

    ui_->hitButton->setVisible(false);
    ui_->standButton->setVisible(false);

    scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(0, 0, 800, 800);
    ui_->graphicsView->setScene(scene_);

    deckPos_ = QPoint(375, 0);

    // Place "deck"
    deckItem_ = scene_->addPixmap(cardSprites_.back());
    deckItem_->setPos(deckPos_);

    view_ = ui_->graphicsView;

    // Set up current bet map
    currentBet_[1] = 0;
    currentBet_[5] = 0;
    currentBet_[10] = 0;
    currentBet_[25] = 0;
    currentBet_[50] = 0;
    currentBet_[100] = 0;

    // Set up connections for chip buttons
    connect(ui_->chip1Button, &QPushButton::clicked, this, [this]() { addChip(1); });
    connect(ui_->chip5Button, &QPushButton::clicked, this, [this]() { addChip(5); });
    connect(ui_->chip10Button, &QPushButton::clicked, this, [this]() { addChip(10); });
    connect(ui_->chip25Button, &QPushButton::clicked, this, [this]() { addChip(25); });
    connect(ui_->chip50Button, &QPushButton::clicked, this, [this]() { addChip(50); });
    connect(ui_->chip100Button, &QPushButton::clicked, this, [this]() { addChip(100); });

    connect(ui_->betDisplay1Button, &QPushButton::clicked, this, [this]() { removeChip(1); });
    connect(ui_->betDisplay5Button, &QPushButton::clicked, this, [this]() { removeChip(5); });
    connect(ui_->betDisplay10Button, &QPushButton::clicked, this, [this]() { removeChip(10); });
    connect(ui_->betDisplay25Button, &QPushButton::clicked, this, [this]() { removeChip(25); });
    connect(ui_->betDisplay50Button, &QPushButton::clicked, this, [this]() { removeChip(50); });
    connect(ui_->betDisplay100Button, &QPushButton::clicked, this, [this]() { removeChip(100); });

    // Starting game.
    connect(ui_->startRoundButton, &QPushButton::clicked, this, &GameWidget::onStartButtonClicked);

    // Card deals.
    connect(game, &BlackjackGame::playerCardDealt, this, &GameWidget::onPlayerCardDealt);
    connect(game, &BlackjackGame::dealerCardDealt, this, &GameWidget::onDealerCardDealt);
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
    // Update current bet
    currentBetTotal_ += value;
    currentBet_[value]++;

    // Add button to view so players can remove chip from bet, and show number
    // of chips of that value currently in bet
    switch (value) {
    case 1:
        ui_->betDisplay1Button->setVisible(true);
        ui_->betDisplay1Button->setText("$1 (" + QString::number(currentBet_[value]) + ")");
        break;
    case 5:
        ui_->betDisplay5Button->setVisible(true);
        ui_->betDisplay5Button->setText("$5 (" + QString::number(currentBet_[value]) + ")");
        break;
    case 10:
        ui_->betDisplay10Button->setVisible(true);
        ui_->betDisplay10Button->setText("$10 (" + QString::number(currentBet_[value]) + ")");
        break;
    case 25:
        ui_->betDisplay25Button->setVisible(true);
        ui_->betDisplay25Button->setText("$25 (" + QString::number(currentBet_[value]) + ")");
        break;
    case 50:
        ui_->betDisplay50Button->setVisible(true);
        ui_->betDisplay50Button->setText("$50 (" + QString::number(currentBet_[value]) + ")");
        break;
    case 100:
        ui_->betDisplay100Button->setVisible(true);
        ui_->betDisplay100Button->setText("$100 (" + QString::number(currentBet_[value]) + ")");
        break;
    default:
        break;  // Should never run
    }

    // Show and update current bet label
    ui_->betLabel->setVisible(true);
    ui_->betLabel->setText("-$" + QString::number(currentBetTotal_));

    // Disable any necessary buttons
    setChipButtonsEnabled();
}

void GameWidget::removeChip(int value) {
    // Update current bet
    currentBetTotal_ -= value;
    int newChipCount = --currentBet_[value];

    // Update label on remove button
    switch (value) {
    case 1:
        if (newChipCount == 0)
            ui_->betDisplay1Button->setVisible(false);
        else
            ui_->betDisplay1Button->setText("$1 (" + QString::number(newChipCount) + ")");
        break;
    case 5:
        if (newChipCount == 0)
            ui_->betDisplay5Button->setVisible(false);
        else
            ui_->betDisplay5Button->setText("$5 (" + QString::number(newChipCount) + ")");
        break;
    case 10:
        if (newChipCount == 0)
            ui_->betDisplay10Button->setVisible(false);
        else
            ui_->betDisplay10Button->setText("$10 (" + QString::number(newChipCount) + ")");
        break;
    case 25:
        if (newChipCount == 0)
            ui_->betDisplay25Button->setVisible(false);
        else
            ui_->betDisplay25Button->setText("$25 (" + QString::number(newChipCount) + ")");
        break;
    case 50:
        if (newChipCount == 0)
            ui_->betDisplay50Button->setVisible(false);
        else
            ui_->betDisplay50Button->setText("$50 (" + QString::number(newChipCount) + ")");
        break;
    case 100:
        if (newChipCount == 0)
            ui_->betDisplay100Button->setVisible(false);
        else
            ui_->betDisplay100Button->setText("$100 (" + QString::number(newChipCount) + ")");
        break;
    default:
        break;  // Should never run
    }

    // Update current bet label and hide if necessary
    if (currentBetTotal_ == 0)
        ui_->betLabel->setVisible(false);
    else
        ui_->betLabel->setText("-$" + QString::number(currentBetTotal_));

    // Enable any necessary chip buttons
    setChipButtonsEnabled();
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

void GameWidget::onStartButtonClicked() {
    ui_->startRoundButton->setVisible(false);
    ui_->hitButton->setVisible(true);
    ui_->standButton->setVisible(true);
    game_->gameStart();
}

void GameWidget::onPlayerCardDealt(Card card){
    QPixmap backPix = cardSprites_.back();
    auto* item = scene_->addPixmap(backPix);
    item->setPos(deckPos_);

    // Changes based on "index" here.
    //                            |
    //                            V
    QPoint handPosition(100 + playerHandIndex_ * 80, 375);
    playerHandIndex_++;
    QPoint belowPosition(375, 60);

    QVariantAnimation* drawPlayerCard = new QVariantAnimation(this);
    drawPlayerCard->setDuration(150);
    drawPlayerCard->setStartValue(deckPos_);
    drawPlayerCard->setEndValue(belowPosition);

    QVariantAnimation* dealPlayerCard = new QVariantAnimation(this);
    dealPlayerCard->setDuration(300);
    dealPlayerCard->setStartValue(belowPosition);
    dealPlayerCard->setEndValue(handPosition);

    connect(drawPlayerCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    connect(dealPlayerCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    connect(drawPlayerCard, &QVariantAnimation::finished, this, [dealPlayerCard] {
        dealPlayerCard->start(QAbstractAnimation::DeleteWhenStopped);
    });

    drawPlayerCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWidget::onDealerCardDealt(Card card){
    QPixmap backPix = cardSprites_.back();
    auto* item = scene_->addPixmap(backPix);
    item->setPos(deckPos_);

    // Changes based on "index" here.
    //                            |
    //                            V
    QPoint handPosition(100 + dealerHandIndex_ * 80, 100);
    dealerHandIndex_++;
    QPoint belowPosition(375, 60);

    QVariantAnimation* drawDealerCard = new QVariantAnimation(this);
    drawDealerCard->setDuration(150);
    drawDealerCard->setStartValue(deckPos_);
    drawDealerCard->setEndValue(belowPosition);

    QVariantAnimation* dealDealerCard = new QVariantAnimation(this);
    dealDealerCard->setDuration(300);
    dealDealerCard->setStartValue(belowPosition);
    dealDealerCard->setEndValue(handPosition);

    connect(drawDealerCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    connect(dealDealerCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    connect(drawDealerCard, &QVariantAnimation::finished, this, [dealDealerCard] {
        dealDealerCard->start(QAbstractAnimation::DeleteWhenStopped);
    });

    drawDealerCard->start(QAbstractAnimation::DeleteWhenStopped);
}

#include "game_widget.h"
#include "ui_game_widget.h"

GameWidget::GameWidget(BlackjackGame* game, QWidget *parent)
    : QWidget(parent), ui_(new Ui::GameWidget), game_(game),
    cardSprites_(":/images/cards.png", 2.0), balance_(1000), playerHandIndex_(0),
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

    // Configure view for proper scaling behavior
    view_->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    view_->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    view_->setRenderHint(QPainter::SmoothPixmapTransform, true);
    view_->setRenderHint(QPainter::Antialiasing, true);
    view_->setCacheMode(QGraphicsView::CacheBackground);

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

    // Button presses.
    connect(ui_->hitButton, &QPushButton::clicked, game_, &BlackjackGame::playerHit);
    connect(ui_->standButton, &QPushButton::clicked, game_, &BlackjackGame::playerStand);
    connect(ui_->doubleButton, &QPushButton::clicked, game_, &BlackjackGame::playerDouble);
    connect(ui_->splitButton, &QPushButton::clicked, game_, &BlackjackGame::playerSplit);
}

GameWidget::~GameWidget()
{
    delete ui_;
}

void GameWidget::beginBetStage() {
    // Show betting buttons
    ui_->startRoundButton->setVisible(true);
    ui_->startRoundButton->setEnabled(false);
    ui_->chip1Button->setVisible(true);
    ui_->chip5Button->setVisible(true);
    ui_->chip10Button->setVisible(true);
    ui_->chip25Button->setVisible(true);
    ui_->chip50Button->setVisible(true);
    ui_->chip100Button->setVisible(true);
    setChipButtonsEnabled();

    // Hide bet text (without shifting layout)
    ui_->betLabel->setVisible(true);
    ui_->betLabel->setText("");

    // Hide gameplay buttons
    ui_->hitButton->setVisible(false);
    ui_->standButton->setVisible(false);
    ui_->doubleButton->setVisible(false);
    ui_->splitButton->setVisible(false);
}

void GameWidget::addChip(int value) {
    // Update current bet
    currentBetTotal_ += value;
    currentBet_[value]++;

    // Enable "Start Round" button
    ui_->startRoundButton->setEnabled(true);

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

    // Update current bet label
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

    // Update current bet label and disable start button if necessary
    if (currentBetTotal_ == 0) {
        ui_->betLabel->setText("");
        ui_->startRoundButton->setEnabled(false);
    }
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
    ui_->doubleButton->setVisible(true);
    ui_->splitButton->setVisible(true);

    // remove betting buttons
    ui_->chip1Button->setVisible(false);
    ui_->chip5Button->setVisible(false);
    ui_->chip10Button->setVisible(false);
    ui_->chip25Button->setVisible(false);
    ui_->chip50Button->setVisible(false);
    ui_->chip100Button->setVisible(false);

    game_->gameStart();
}

void GameWidget::onPlayerCardDealt(Card card){
    QPixmap backPix = cardSprites_.back();
    QGraphicsPixmapItem* item = scene_->addPixmap(backPix);
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

    connect(dealPlayerCard, &QVariantAnimation::finished, this, [this, item, card]() {
        flipCard(item, card);
    });

    drawPlayerCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWidget::onDealerCardDealt(Card card){
    QPixmap backPix = cardSprites_.back();
    QGraphicsPixmapItem* item = scene_->addPixmap(backPix);
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

    connect(dealDealerCard, &QVariantAnimation::finished, this, [this, item, card]() {
        if(dealerHandIndex_ > 1){
            flipCard(item, card);
        }
    });

    drawDealerCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWidget::flipCard(QGraphicsPixmapItem* item, const Card& card){
    // Since everything is 2D there is no actual flipping, so instead I did a cool shrinking thing I saw online
    // and wanted to replicate.

    // This was kinda hard to do and follow so I'll leave comments so that the group can understand easier.

    // This flips around the vertical center.
    item->setTransformOriginPoint(item->boundingRect().center());

    // First this animates shrinking x axis.
    QVariantAnimation* shrink = new QVariantAnimation(this);
    shrink->setDuration(150);
    shrink->setStartValue(1.0);
    shrink->setEndValue(0.0);

    connect(shrink, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        const qreal s = v.toReal();
        item->setScale(s);
    });

    // Once shrunk, this swaps pixmap to correct card face and grow again.
    connect(shrink, &QVariantAnimation::finished, this, [this, item, card]() {
        item->setPixmap(cardSprites_.faceFor(card));

        QVariantAnimation* grow = new QVariantAnimation(this);
        grow->setDuration(150);
        grow->setStartValue(0.0);
        grow->setEndValue(1.0);

        connect(grow, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
            const qreal s = v.toReal();
            item->setScale(s);
        });

        grow->start(QAbstractAnimation::DeleteWhenStopped);
    });

    shrink->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateViewScale();
}

void GameWidget::updateViewScale() {
    if (!view_ || !scene_) {
        return;
    }

    // Fit the 800x800 scene into the available space while maintaining aspect ratio
    view_->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
}

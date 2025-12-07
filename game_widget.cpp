#include "game_widget.h"
#include "ui_game_widget.h"

GameWidget::GameWidget(BlackjackGame* game, QWidget *parent)
    : QWidget(parent), ui_(new Ui::GameWidget), game_(game),
    cardSprites_(":/images/cards.png", 2.0), balance_(1000),
    currentBetTotal_(0), holeCard_(Card::Rank::Cut, Card::Suit::Cut) {
    ui_->setupUi(this);

    ui_->hitButton->setVisible(false);
    ui_->standButton->setVisible(false);

    scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    ui_->graphicsView->setScene(scene_);

    deckPos_ = QPoint(SCENE_WIDTH - 50, 0);

    // Place "deck" in top-right corner
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
    connect(ui_->betAllButton, &QPushButton::clicked, this, &GameWidget::onAllIn);

    connect(ui_->betDisplay1Button, &QPushButton::clicked, this, [this]() { removeChip(1); });
    connect(ui_->betDisplay5Button, &QPushButton::clicked, this, [this]() { removeChip(5); });
    connect(ui_->betDisplay10Button, &QPushButton::clicked, this, [this]() { removeChip(10); });
    connect(ui_->betDisplay25Button, &QPushButton::clicked, this, [this]() { removeChip(25); });
    connect(ui_->betDisplay50Button, &QPushButton::clicked, this, [this]() { removeChip(50); });
    connect(ui_->betDisplay100Button, &QPushButton::clicked, this, [this]() { removeChip(100); });

    // TODO: change the placement of the connect calls below this comment
    // Starting/ending game.
    connect(ui_->startRoundButton, &QPushButton::clicked, this, &GameWidget::onStartButtonClicked);
    connect(this, &GameWidget::beginRound, game_, &BlackjackGame::beginRound);
    connect(game_, &BlackjackGame::roundEnded, this, &GameWidget::onRoundEnded);
    connect(game_, &BlackjackGame::playerTurn, this, &GameWidget::onPlayerTurn);
    connect(game_, &BlackjackGame::dealerTurnStarted, this, &GameWidget::onDealerTurnStarted);
    connect(game_, &BlackjackGame::betPlaced, this, &GameWidget::onBetPlaced);
    connect(game_, &BlackjackGame::splitHand, this, &GameWidget::onHandSplit);

    // Card deals.
    connect(game, &BlackjackGame::playerCardDealt, this, &GameWidget::onPlayerCardDealt);
    connect(game, &BlackjackGame::dealerCardDealt, this, &GameWidget::onDealerCardDealt);

    // Button presses.
    connect(ui_->hitButton, &QPushButton::clicked, game_, &BlackjackGame::playerHit);
    connect(ui_->standButton, &QPushButton::clicked, game_, &BlackjackGame::playerStand);
    connect(ui_->doubleButton, &QPushButton::clicked, game_, &BlackjackGame::playerDouble);
    connect(ui_->splitButton, &QPushButton::clicked, game_, &BlackjackGame::playerSplit);
    connect(ui_->returnButton, &QPushButton::clicked, this, &GameWidget::onReturnToMainMenu);
}

GameWidget::~GameWidget()
{
    delete ui_;
}

void GameWidget::onRoundEnded(BlackjackGame::GameResult result, int payout,
                               int handIndex, int totalHands) {
    QString message;

    switch (result) {
    case BlackjackGame::GameResult::Win:
        message = "Win";
        break;
    case BlackjackGame::GameResult::Lose:
        message = "Lose";
        break;
    case BlackjackGame::GameResult::Push:
        message = "Push";
        break;
    case BlackjackGame::GameResult::Blackjack:
        message = "Blackjack";
        break;
    }

    // Add hand number if multiple hands
    if (totalHands > 1) {
        message = QString("Hand %1: %2").arg(handIndex + 1).arg(message);
    }

    // Update balance label.
    balance_ += payout;
    ui_->balanceLabel->setText("$" + QString::number(balance_));

    if (balance_ == 0) {
        timer_.singleShot(2000, this, [this]() {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("YOU HAVE NO MONEY!");
            msgBox.setText("Do you want to restart?");

            QPushButton* restartButton =
                msgBox.addButton("Yes, restart", QMessageBox::AcceptRole);
            QPushButton* menuButton =
                msgBox.addButton("No, return to menu", QMessageBox::RejectRole);

            msgBox.exec();

            if (msgBox.clickedButton() == restartButton) {
                balance_ = 1000;
                ui_->balanceLabel->setText("$" + QString::number(balance_));
                GameWidget::resetGame();
            }
            else if (msgBox.clickedButton() == menuButton) {
                emit returnToMainMenu();
            }
        });
    }

    // Indicate results in bet label.
    ui_->betLabel->setVisible(true);
    ui_->betLabel->setText(message);

    // Only reset game after the LAST hand is processed
    if (handIndex == totalHands - 1) {
        // Add extra delay after last hand before reset
        timer_.singleShot(2000, this, &GameWidget::resetGame);
    }
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
    ui_->betAllButton->setVisible(true);
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
    // Hide start round button
    ui_->startRoundButton->setVisible(false);

    // Gameplay buttons will be shown by onTurnChanged signal

    // remove betting buttons
    ui_->chip1Button->setVisible(false);
    ui_->chip5Button->setVisible(false);
    ui_->chip10Button->setVisible(false);
    ui_->chip25Button->setVisible(false);
    ui_->chip50Button->setVisible(false);
    ui_->chip100Button->setVisible(false);
    ui_->betAllButton->setVisible(false);

    // Hide bet label
    ui_->betLabel->setText("");

    emit beginRound(currentBetTotal_);

    // Update balance label with current balance after bet
    currentBet_[1] = 0;
    currentBet_[5] = 0;
    currentBet_[10] = 0;
    currentBet_[25] = 0;
    currentBet_[50] = 0;
    currentBet_[100] = 0;   // TODO: refactor this into a helper method
    currentBetTotal_ = 0;
}

void GameWidget::onPlayerCardDealt(Card card, int handIndex, bool isLastCard) {
    // Create card item at deck position
    QPixmap backPix = cardSprites_.back();
    QGraphicsPixmapItem* item = scene_->addPixmap(backPix);
    item->setPos(deckPos_);

    // Rotate if last card (doubled/split aces)
    if (isLastCard)
        item->setRotation(-90);

    // Add a new hand if this is the first card being dealt
    if (playerHandCards_.empty()) {
        playerHandCards_.append(QVector<QGraphicsPixmapItem*>());
    }
    playerHandCards_[handIndex].append(item);

    // Calculate position using horizontal distribution
    int numCards = playerHandCards_[handIndex].size();
    int totalHands = playerHandCards_.size();

    // Get hand base position
    QVector<int> handBasePositions = calculateHandBaseXPositions(totalHands);
    int handBaseX = handBasePositions[handIndex];

    // Get relative positions for cards in this hand
    QVector<int> relativePositions = calculateRelativeCardPositions(numCards);

    // This card's final position (last card in hand)
    int finalX = handBaseX + relativePositions.last();

    // All player hands at same Y: PLAYER_HAND_Y
    QPoint handPosition(finalX, PLAYER_HAND_Y);
    QPoint belowPosition(deckPos_.x(), deckPos_.y() + DECK_DRAW_OFFSET);

    // Animation 1: Draw from deck to gathering point (150ms)
    QVariantAnimation* drawPlayerCard = new QVariantAnimation(this);
    drawPlayerCard->setDuration(DECK_DRAW_DURATION);
    drawPlayerCard->setStartValue(deckPos_);
    drawPlayerCard->setEndValue(belowPosition);

    connect(drawPlayerCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    // Animation 2: Deal from gathering point to hand position (300ms)
    QVariantAnimation* dealPlayerCard = new QVariantAnimation(this);
    dealPlayerCard->setDuration(DEAL_TO_HAND_DURATION);
    dealPlayerCard->setStartValue(belowPosition);
    dealPlayerCard->setEndValue(handPosition);

    connect(dealPlayerCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    // Connect animations in sequence
    connect(drawPlayerCard, &QVariantAnimation::finished, this, [dealPlayerCard] {
        dealPlayerCard->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // On completion: flip card and reposition existing cards to maintain centering
    connect(dealPlayerCard, &QVariantAnimation::finished, this, [this, item, card, handIndex]() {
        flipCard(item, card);
        // Reposition all cards in hand to keep centered (200ms smooth shift)
        repositionHandCards(handIndex, 200);
    });

    drawPlayerCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWidget::onDealerCardDealt(Card card) {
    // Create card item at deck position
    QPixmap backPix = cardSprites_.back();
    QGraphicsPixmapItem* item = scene_->addPixmap(backPix);
    item->setPos(deckPos_);

    // Add to dealer hand tracking
    dealerHandCards_.append(item);

    // Calculate final centered positions for dealer hand
    int numCards = dealerHandCards_.size();
    QVector<int> xPositions = calculateCenteredPositions(numCards);

    // All dealer hands at same Y: DEALER_HAND_Y
    // This card's final position
    QPoint handPosition(xPositions.last(), DEALER_HAND_Y);
    QPoint belowPosition(deckPos_.x(), deckPos_.y() + DECK_DRAW_OFFSET);

    // Animation 1: Draw from deck to gathering point
    QVariantAnimation* drawDealerCard = new QVariantAnimation(this);
    drawDealerCard->setDuration(DECK_DRAW_DURATION);
    drawDealerCard->setEasingCurve(QEasingCurve::InOutExpo);
    drawDealerCard->setStartValue(deckPos_);
    drawDealerCard->setEndValue(belowPosition);

    connect(drawDealerCard, &QVariantAnimation::valueChanged,
            this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    // Animation 2: Deal from gathering point to hand position
    QVariantAnimation* dealDealerCard = new QVariantAnimation(this);
    dealDealerCard->setDuration(DEAL_TO_HAND_DURATION);
    dealDealerCard->setEasingCurve(QEasingCurve::InOutExpo);
    dealDealerCard->setStartValue(belowPosition);
    dealDealerCard->setEndValue(handPosition);

    connect(dealDealerCard, &QVariantAnimation::valueChanged,
            this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    // Connect animations in sequence
    connect(drawDealerCard, &QVariantAnimation::finished,
            this, [dealDealerCard] {
        dealDealerCard->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // On completion: handle hole card or flip, then reposition
    connect(dealDealerCard, &QVariantAnimation::finished,
            this, [this, item, card]() {
        // Check if this is the second card (hole card)
        if (dealerHandCards_.size() == 2) {
            // Save hole card for later flip
            holeCard_ = card;
            holeCardItem_ = item;
        } else {
            // Flip regular dealer card
            flipCard(item, card);
        }
        // Reposition dealer cards to maintain centering
        repositionHandCards(-1, 200);  // -1 indicates dealer hand
    });

    drawDealerCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWidget::flipCard(QGraphicsPixmapItem* item, const Card& card) {
    // Since everything is 2D there is no actual flipping, so instead I did a cool shrinking thing I saw online
    // and wanted to replicate.

    // This was kinda hard to do and follow so I'll leave comments so that the group can understand easier.

    // This flips around the vertical center.
    item->setTransformOriginPoint(item->boundingRect().center());

    // First this animates shrinking x axis.
    QVariantAnimation* shrink = new QVariantAnimation(this);
    shrink->setDuration(FLIP_DURATION);
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

QVector<int> GameWidget::calculateCenteredPositions(int numCards) const {
    if (numCards == 0) return QVector<int>();

    const int sceneWidth = 800;
    const int cardWidth = 71;   // Width of card sprite
    const int cardGap = 10;     // Gap between cards (reduced from 80)

    // Calculate total width needed for all cards
    int totalWidth = (numCards * cardWidth) + ((numCards - 1) * cardGap);

    // Calculate starting X to center the hand
    int startX = (sceneWidth - totalWidth) / 2;

    // Generate X position for each card
    QVector<int> positions;
    for (int i = 0; i < numCards; i++) {
        positions.append(startX + (i * (cardWidth + cardGap)));
    }

    return positions;
}

QVector<int> GameWidget::calculateHandBaseXPositions(int totalHands) const {
    if (totalHands == 0) return QVector<int>();

    QVector<int> basePositions;

    for (int i = 0; i < totalHands; i++) {
        // Center of each section: (width / N) * (i + 0.5)
        int centerX = (SCENE_WIDTH * (i + 0.5)) / totalHands;
        basePositions.append(centerX);
    }

    return basePositions;
}

QVector<int> GameWidget::calculateRelativeCardPositions(int numCards) const {
    if (numCards == 0) return QVector<int>();

    const int cardWidth = 71;
    const int cardGap = 10;

    // Total width of all cards plus gaps
    int totalWidth = (numCards * cardWidth) + ((numCards - 1) * cardGap);

    // Starting offset from hand center (negative = left of center)
    int startOffset = -totalWidth / 2;

    QVector<int> relativePositions;
    for (int i = 0; i < numCards; i++) {
        relativePositions.append(startOffset + (i * (cardWidth + cardGap)));
    }

    return relativePositions;
}

void GameWidget::repositionHandCards(int handIndex, int duration) {
    QVector<QGraphicsPixmapItem*> cards;

    if (handIndex == -1) {
        // Dealer hand - unchanged behavior
        cards = dealerHandCards_;

        if (cards.isEmpty()) return;

        // Dealer always centered
        QVector<int> newXPositions = calculateCenteredPositions(cards.size());

        for (int i = 0; i < cards.size(); i++) {
            QGraphicsPixmapItem* card = cards[i];
            QPointF currentPos = card->pos();
            QPointF newPos(newXPositions[i], DEALER_HAND_Y);

            if (currentPos == newPos) continue;

            QVariantAnimation* reposition = new QVariantAnimation(this);
            reposition->setDuration(duration);
            reposition->setStartValue(currentPos);
            reposition->setEndValue(newPos);
            reposition->setEasingCurve(QEasingCurve::InOutQuad);

            connect(reposition, &QVariantAnimation::valueChanged,
                    this, [card](const QVariant& v) {
                card->setPos(v.toPointF());
            });

            reposition->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else {
        // Player hand - NEW horizontal distribution logic
        if (handIndex >= playerHandCards_.size()) return;
        cards = playerHandCards_[handIndex];

        if (cards.isEmpty()) return;

        // Calculate horizontal distribution
        int totalHands = playerHandCards_.size();
        QVector<int> handBasePositions = calculateHandBaseXPositions(totalHands);
        int handBaseX = handBasePositions[handIndex];

        // Calculate relative card positions
        QVector<int> relativePositions = calculateRelativeCardPositions(cards.size());

        // Animate each card
        for (int i = 0; i < cards.size(); i++) {
            QGraphicsPixmapItem* card = cards[i];
            QPointF currentPos = card->pos();

            // Combine hand base position with card offset
            int finalX = handBaseX + relativePositions[i];
            QPointF newPos(finalX, PLAYER_HAND_Y);

            if (currentPos == newPos) continue;

            QVariantAnimation* reposition = new QVariantAnimation(this);
            reposition->setDuration(duration);
            reposition->setStartValue(currentPos);
            reposition->setEndValue(newPos);
            reposition->setEasingCurve(QEasingCurve::InOutQuad);

            connect(reposition, &QVariantAnimation::valueChanged,
                    this, [card](const QVariant& v) {
                card->setPos(v.toPointF());
            });

            reposition->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

void GameWidget::resetGame() {
    // Reset the scene.
    scene_->clear();
    deckItem_ = scene_->addPixmap(cardSprites_.back());
    deckItem_->setPos(deckPos_);

    // Clear card tracking structures
    playerHandCards_.clear();
    dealerHandCards_.clear();

    // Reset variables.
    currentBetTotal_ = 0;

    // Reset chips.
    QMapIterator<int, int> i(currentBet_);
    while (i.hasNext()) {
        i.next();
        currentBet_[i.key()] = 0;
    }

    // Hide chip buttons.
    ui_->betDisplay1Button->setVisible(false);
    ui_->betDisplay5Button->setVisible(false);
    ui_->betDisplay10Button->setVisible(false);
    ui_->betDisplay25Button->setVisible(false);
    ui_->betDisplay50Button->setVisible(false);
    ui_->betDisplay100Button->setVisible(false);

    beginBetStage();
}

void GameWidget::onPlayerTurn(int handIndex, bool canDouble, bool canSplit) {
    // Track which hand is active to support multi-hand logic
    currentHandIndex_ = handIndex;

    // Show gameplay buttons for player's turn
    ui_->hitButton->setVisible(true);
    ui_->standButton->setVisible(true);

    // Show double/split buttons based on game logic decision
    ui_->doubleButton->setVisible(canDouble);
    ui_->splitButton->setVisible(canSplit);
}

void GameWidget::onDealerTurnStarted() {
    // Hide all gameplay buttons during dealer's turn
    ui_->hitButton->setVisible(false);
    ui_->standButton->setVisible(false);
    ui_->doubleButton->setVisible(false);
    ui_->splitButton->setVisible(false);

    // Disable chip buttons (but keep them visible)
    ui_->betDisplay1Button->setEnabled(false);
    ui_->betDisplay5Button->setEnabled(false);
    ui_->betDisplay10Button->setEnabled(false);
    ui_->betDisplay25Button->setEnabled(false);
    ui_->betDisplay50Button->setEnabled(false);
    ui_->betDisplay100Button->setEnabled(false);

    // Flip hole card
    flipCard(holeCardItem_, holeCard_);
}

void GameWidget::onHandSplit(int handIndex) {
    playerHandCards_.insert(handIndex + 1, QVector<QGraphicsPixmapItem*>());

    // Move the second card from the hand being split into the new hand
    QGraphicsPixmapItem* splitCardItem = playerHandCards_[handIndex].takeAt(1);
    playerHandCards_[handIndex + 1].append(splitCardItem);

    // if (totalHands >= 2 && playerHandCards_.contains(0) && playerHandCards_.contains(1)) {
    //     // Move the second card item from hand 0 to be the first card in hand 1
    //     if (playerHandCards_[0].size() >= 3 && playerHandCards_[1].size() >= 1) {
    //         // The split card is the second card in hand 0 (index 1)
    //         QGraphicsPixmapItem* splitCardItem = playerHandCards_[0][1];

    //         // Remove from hand 0
    //         playerHandCards_[0].remove(1);

    //         // Insert at beginning of hand 1 (before the newly dealt card)
    //         playerHandCards_[1].prepend(splitCardItem);
    //     }
    // }

    // Now reposition all player hands to distribute horizontally
    for (int handIndex = 0; handIndex < playerHandCards_.size(); handIndex++) {
        repositionHandCards(handIndex, 400);
    }
}

void GameWidget::onBetPlaced(int betAmount) {
    balance_ -= betAmount;
    ui_->betLabel->setText("-$" + QString::number(betAmount));
    timer_.singleShot(2000, [this]() {
        ui_->balanceLabel->setText("$" + QString::number(balance_));
        ui_->betLabel->setText("");
    });
}

void GameWidget::onReturnToMainMenu(){
 QMessageBox::StandardButton reply = QMessageBox::question(this, "Exiting main menu?", "Are you sure you want to return to the main menu?",
                                                                QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit returnToMainMenu();
    }
    else if (reply == QMessageBox::No) {
        // Do nothing pretty much.
    }
}

void GameWidget::onAllIn(){
    int remaining = balance_ - currentBetTotal_;
    if (remaining <= 0) {
        return;
    }

    const int chipValues[] = {100, 50, 25, 10, 5, 1};

    for (int value : chipValues) {
        while (remaining >= value) {
            addChip(value);
            remaining -= value;
        }
    }
}

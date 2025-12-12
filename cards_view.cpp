#include "cards_view.h"
#include <QResizeEvent>

CardsView::CardsView(QWidget* parent)
    : QWidget(parent), cardSprites_(":/images/cards.png", 1.0),
    deckItem_(nullptr), cutCardItem_(nullptr), currentHandIndex_(0),
    hasSplit_(false), holeCardItem_(nullptr), // Card doesn't matter
    holeCard_(Card::Rank::Cut, Card::Suit::Cut), cardScale_(1.0) {
    // Create internal graphics view and scene
    view_ = new QGraphicsView(this);
    scene_ = new QGraphicsScene(this);

    // Set scene on view
    view_->setScene(scene_);

    // Configure view settings
    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    view_->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    view_->setRenderHint(QPainter::SmoothPixmapTransform, true);
    view_->setRenderHint(QPainter::Antialiasing, true);
    view_->setCacheMode(QGraphicsView::CacheBackground);

    // Initial scene size matches widget size (will be updated in resizeEvent)
    scene_->setSceneRect(0, 0, width(), height());
    view_->setGeometry(0, 0, width(), height());

    // Calculate initial deck position
    updateDeckPosition();

    // Create deck item at calculated position
    deckItem_ = scene_->addPixmap(cardSprites_.back());
    deckItem_->setTransformOriginPoint(deckItem_->boundingRect().center());
    deckItem_->setPos(deckPos_);

    // Calculate and apply initial card scale
    cardScale_ = calculateCardScale();
    if (deckItem_) {
        deckItem_->setScale(cardScale_);
    }
}

int CardsView::getDealerHandY() const {
    return static_cast<int>(scene_->sceneRect().height() * DEALER_Y_PERCENT);
}

int CardsView::getPlayerHandY() const {
    return static_cast<int>(scene_->sceneRect().height() * PLAYER_Y_PERCENT);
}

float CardsView::calculateCardScale() const {
    // Reference dimensions (what card sizes are designed for)
    // TODO: design a cleaner implementation that doesn't need arbitrary reference dimensions
    const float refWidth = 960.0;
    const float refHeight = 540.0;

    // Calculate scale based on current scene size
    float scaleX = scene_->sceneRect().width() / refWidth;
    float scaleY = scene_->sceneRect().height() / refHeight;

    // Use minimum to ensure cards fit in both dimensions
    return qMin(scaleX, scaleY);
}

void CardsView::scaleAllCards() {
    // Scale deck and cut card
    if (deckItem_) {
        deckItem_->setScale(cardScale_);
    }
    if (cutCardItem_) {
        cutCardItem_->setScale(cardScale_);
    }

    // Scale all dealer cards
    for (QGraphicsPixmapItem* card : dealerHandCards_) {
        card->setScale(cardScale_);
    }

    // Scale all player cards
    for (const QVector<QGraphicsPixmapItem*>& hand : playerHandCards_) {
        for (QGraphicsPixmapItem* card : hand) {
            card->setScale(cardScale_);
        }
    }
}

void CardsView::updateDeckPosition() {
    // Calculate where we want the center of the scaled card to appear in scene coordinates
    float desiredCenterX = scene_->width() - DECK_RIGHT_MARGIN - (CARD_WIDTH * cardScale_ / 2.0);
    float desiredCenterY = DECK_TOP_MARGIN + (CARD_HEIGHT * cardScale_ / 2.0);

    // Transform origin is at (CARD_WIDTH/2, CARD_HEIGHT/2) in unscaled item coordinates
    // setPos() positions the item's (0,0) point, so we must offset by the transform origin
    deckPos_ = QPoint(
        static_cast<int>(desiredCenterX - CARD_WIDTH / 2.0),
        static_cast<int>(desiredCenterY - CARD_HEIGHT / 2.0)
    );

    // Update deck item position if it exists
    if (deckItem_) {
        deckItem_->setPos(deckPos_);
    }

    // Update cut card position if it exists
    if (cutCardItem_) {
        cutCardItem_->setPos(deckPos_.x(), deckPos_.y() + CUT_CARD_OFFSET_Y);
    }
}

QVector<int> CardsView::calculateHandBaseXPositions(int totalHands) const {
    if (totalHands == 0) return QVector<int>();

    QVector<int> basePositions;
    int sceneWidth = static_cast<int>(scene_->sceneRect().width());

    for (int i = 0; i < totalHands; i++) {
        // Center of each section: (width / N) * (i + 0.5)
        int centerX = (sceneWidth * (i * 2 + 1)) / (totalHands * 2);
        basePositions.append(centerX);
    }

    return basePositions;
}

QVector<int> CardsView::calculateRelativeCardPositions(int numCards) const {
    if (numCards == 0) return QVector<int>();

    // Use scaled card dimensions
    int scaledCardWidth = static_cast<int>(CARD_WIDTH * cardScale_);
    int scaledGap = static_cast<int>(CARD_GAP * scaledCardWidth);

    // Gap between center of leftmost card and center of rightmost card
    int totalWidth = (numCards - 1) * scaledGap;

    // Starting offset from hand center (negative = left of center)
    int startOffset = -totalWidth / 2;

    QVector<int> relativePositions;
    for (int i = 0; i < numCards; i++) {
        relativePositions.append(startOffset + (i * scaledGap));
    }

    return relativePositions;
}

void CardsView::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    // Resize internal view to fill widget
    view_->setGeometry(rect());

    // Resize scene to match widget dimensions (no letterboxing)
    scene_->setSceneRect(rect());

    // Calculate new card scale factor
    cardScale_ = calculateCardScale();

    // Scale all existing cards
    scaleAllCards();

    // Update deck and cut card positions based on new scene size
    updateDeckPosition();

    // Reposition all hands with scaled card dimensions
    repositionAllHands();

    // Reposition hand selection.
    updateHandSelectionPosition();
}

void CardsView::repositionAllHands() {
    // Reposition dealer hand (centered at dynamic Y position)
    if (!dealerHandCards_.isEmpty()) {
        QVector<int> newXPositions = calculateRelativeCardPositions(dealerHandCards_.size());
        int centerX = scene_->width() / 2;
        int dealerY = getDealerHandY();

        for (int i = 0; i < dealerHandCards_.size(); i++) {
            // newXPositions[i] + centerX is the desired center position in scene coords
            // Offset by transform origin to position the (0,0) point correctly
            dealerHandCards_[i]->setPos(
                newXPositions[i] + centerX - CARD_WIDTH / 2,
                dealerY - CARD_HEIGHT / 2
            );
        }
    }

    // Reposition all player hands (distributed horizontally at dynamic Y position)
    for (int handIndex = 0; handIndex < playerHandCards_.size(); handIndex++) {
        if (playerHandCards_[handIndex].isEmpty()) continue;

        // Calculate horizontal distribution
        int totalHands = playerHandCards_.size();
        QVector<int> handBasePositions = calculateHandBaseXPositions(totalHands);
        int handBaseX = handBasePositions[handIndex];

        // Calculate relative card positions
        QVector<int> relativePositions = calculateRelativeCardPositions(playerHandCards_[handIndex].size());
        int playerY = getPlayerHandY();

        // Set each card position directly
        for (int i = 0; i < playerHandCards_[handIndex].size(); i++) {
            // finalX is the desired center X position in scene coords
            int finalX = handBaseX + relativePositions[i];
            // Offset by transform origin to position the (0,0) point correctly
            playerHandCards_[handIndex][i]->setPos(
                finalX - CARD_WIDTH / 2,
                playerY - CARD_HEIGHT / 2
            );
        }
    }
}

void CardsView::repositionHandCards(int handIndex, int duration) {
    QVector<QGraphicsPixmapItem*> cards;

    if (handIndex == -1) {
        // Dealer hand - centered
        cards = dealerHandCards_;
        if (cards.isEmpty()) return;

        QVector<int> newXPositions = calculateRelativeCardPositions(cards.size() + 1);
        int centerX = scene_->width() / 2;
        int dealerY = getDealerHandY();

        for (int i = 0; i < cards.size(); i++) {
            QGraphicsPixmapItem* card = cards[i];
            QPointF currentPos = card->pos();
            // newXPositions[i] + centerX is desired center position
            // Offset by transform origin to position the (0,0) point correctly
            QPointF newPos(
                newXPositions[i] + centerX - CARD_WIDTH / 2,
                dealerY - CARD_HEIGHT / 2
            );

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
        // Player hand - distributed horizontally
        if (handIndex >= playerHandCards_.size()) return;
        cards = playerHandCards_[handIndex];
        if (cards.isEmpty()) return;

        // Calculate horizontal distribution
        int totalHands = playerHandCards_.size();
        QVector<int> handBasePositions = calculateHandBaseXPositions(totalHands);
        int handBaseX = handBasePositions[handIndex];

        // Calculate relative card positions
        QVector<int> relativePositions = calculateRelativeCardPositions(cards.size() + 1);
        int playerY = getPlayerHandY();

        // Animate each card
        for (int i = 0; i < cards.size(); i++) {
            QGraphicsPixmapItem* card = cards[i];
            QPointF currentPos = card->pos();

            // Combine hand base position with card offset to get desired center position
            int finalX = handBaseX + relativePositions[i];
            // Offset by transform origin to position the (0,0) point correctly
            QPointF newPos(
                finalX - CARD_WIDTH / 2,
                playerY - CARD_HEIGHT / 2
            );

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

void CardsView::flipCard(QGraphicsPixmapItem* item, const Card& card) {
    // Animate shrinking card
    QVariantAnimation* shrink = new QVariantAnimation(this);
    shrink->setDuration(FLIP_DURATION);
    shrink->setStartValue(1.0);
    shrink->setEndValue(0.0);

    connect(shrink, &QVariantAnimation::valueChanged, this, [this, item](const QVariant& v) {
        item->setScale(v.toReal() * cardScale_);
    });

    // Replace card back with front, animate growing card
    connect(shrink, &QVariantAnimation::finished, this, [this, item, card]() {
        item->setPixmap(cardSprites_.faceFor(card));

        QVariantAnimation* grow = new QVariantAnimation(this);
        grow->setDuration(FLIP_DURATION);
        grow->setStartValue(0.0);
        grow->setEndValue(1.0);

        connect(grow, &QVariantAnimation::valueChanged, this, [this, item](const QVariant& v) {
            item->setScale(v.toReal() * cardScale_);
        });

        grow->start(QAbstractAnimation::DeleteWhenStopped);
    });

    shrink->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardsView::dealPlayerCard(const Card& card, int handIndex, bool isLastCard) {
    // Create card item at deck position
    QPixmap backPix = cardSprites_.back();
    QGraphicsPixmapItem* item = scene_->addPixmap(backPix);
    item->setTransformOriginPoint(item->boundingRect().center());
    item->setPos(deckPos_);
    item->setScale(cardScale_);

    // Rotate if last card (doubled/split aces)
    if (isLastCard) {
        item->setRotation(-90);
    }

    // Add a new hand if this is the first card being dealt to this index
    while (playerHandCards_.size() <= handIndex) {
        playerHandCards_.append(QVector<QGraphicsPixmapItem*>());
    }

    // Reposition existing cards in this hand BEFORE adding the new card
    // This way only existing cards are repositioned, not the new one being dealt
    if (!playerHandCards_[handIndex].isEmpty()) {
        repositionHandCards(handIndex, DEAL_TO_HAND_DURATION);
    }

    playerHandCards_[handIndex].append(item);

    // Calculate final position
    int numCards = playerHandCards_[handIndex].size();
    int totalHands = playerHandCards_.size();

    QVector<int> handBasePositions = calculateHandBaseXPositions(totalHands);
    int handBaseX = handBasePositions[handIndex];

    QVector<int> relativePositions = calculateRelativeCardPositions(numCards);
    int finalCenterX = handBaseX + relativePositions.last();
    int finalCenterY = getPlayerHandY();

    // Offset by transform origin to position the (0,0) point correctly
    QPoint handPosition(
        finalCenterX - CARD_WIDTH / 2,
        finalCenterY - CARD_HEIGHT / 2
    );
    QPoint drawPoint(deckPos_.x(), deckPos_.y() + DECK_DRAW_OFFSET * cardScale_ * CARD_WIDTH);

    // Animate drawing from deck to draw point (right below deck)
    QVariantAnimation* drawCard = new QVariantAnimation(this);
    drawCard->setDuration(DECK_DRAW_DURATION);
    drawCard->setStartValue(deckPos_);
    drawCard->setEndValue(drawPoint);

    connect(drawCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    // Animate movment from draw point to hand position
    QVariantAnimation* dealCard = new QVariantAnimation(this);
    dealCard->setDuration(DEAL_TO_HAND_DURATION);
    dealCard->setStartValue(drawPoint);
    dealCard->setEndValue(handPosition);

    connect(dealCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    // Connect animations in sequence
    connect(drawCard, &QVariantAnimation::finished, this, [dealCard] {
        dealCard->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // On completion: flip card
    connect(dealCard, &QVariantAnimation::finished, this, [this, item, card]() {
        flipCard(item, card);
    });

    drawCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardsView::dealDealerCard(const Card& card) {
    // Create card item at deck position
    QPixmap backPix = cardSprites_.back();
    QGraphicsPixmapItem* item = scene_->addPixmap(backPix);
    item->setTransformOriginPoint(item->boundingRect().center());
    item->setPos(deckPos_);
    item->setScale(cardScale_);

    // Reposition existing cards as new card is dealt
    if (!dealerHandCards_.isEmpty()) {
        repositionHandCards(-1, DEAL_TO_HAND_DURATION);
    }

    // Add to dealer hand tracking
    dealerHandCards_.append(item);

    // Calculate final centered position for dealer hand
    int numCards = dealerHandCards_.size();
    QVector<int> xOffsets = calculateRelativeCardPositions(numCards);
    int centerX = scene_->width() / 2;
    int dealerCenterY = getDealerHandY();

    // xOffsets.last() + centerX is the desired center position
    // Offset by transform origin to position the (0,0) point correctly
    QPoint handPosition(
        xOffsets.last() + centerX - CARD_WIDTH / 2,
        dealerCenterY - CARD_HEIGHT / 2
    );
    QPoint drawPoint(deckPos_.x(), deckPos_.y() + DECK_DRAW_OFFSET * cardScale_ * CARD_WIDTH);

    // Animate drawing from deck to draw point (right below deck)
    QVariantAnimation* drawCard = new QVariantAnimation(this);
    drawCard->setDuration(DECK_DRAW_DURATION);
    drawCard->setEasingCurve(QEasingCurve::InOutExpo);
    drawCard->setStartValue(deckPos_);
    drawCard->setEndValue(drawPoint);

    connect(drawCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    // Animate movement from draw point to hand position
    QVariantAnimation* dealCard = new QVariantAnimation(this);
    dealCard->setDuration(DEAL_TO_HAND_DURATION);
    dealCard->setEasingCurve(QEasingCurve::InOutExpo);
    dealCard->setStartValue(drawPoint);
    dealCard->setEndValue(handPosition);

    connect(dealCard, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setPos(v.toPointF());
    });

    // Connect animations in sequence
    connect(drawCard, &QVariantAnimation::finished, this, [dealCard] {
        dealCard->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // On completion: handle hole card or flip, then reposition
    connect(dealCard, &QVariantAnimation::finished, this, [this, item, card]() {
        // Check if this is the second card (hole card)
        if (dealerHandCards_.size() == 2) {
            // Save hole card for later flip
            holeCard_ = card;
            holeCardItem_ = item;
        } else {
            // Flip regular dealer card
            flipCard(item, card);
        }
    });

    drawCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardsView::flipDealerHoleCard() {
    if (holeCardItem_) {
        flipCard(holeCardItem_, holeCard_);
    }
}

void CardsView::handleHandSplit(int handIndex) {
    if (handIndex >= playerHandCards_.size() || playerHandCards_[handIndex].size() < 2) {
        return; // Invalid split
    }

    // Insert a new empty hand after the hand being split
    playerHandCards_.insert(handIndex + 1, QVector<QGraphicsPixmapItem*>());

    // Move the second card from the hand being split into the new hand
    QGraphicsPixmapItem* splitCardItem = playerHandCards_[handIndex].takeAt(1);
    playerHandCards_[handIndex + 1].append(splitCardItem);

    hasSplit_ = true;

    if (!handSelectionItem_) {
        QPixmap selectionPixmap(":/images/handSelection.png");
        handSelectionItem_ = scene_->addPixmap(selectionPixmap);
        handSelectionItem_->setTransformOriginPoint(handSelectionItem_->boundingRect().center());
        handSelectionItem_->setScale(cardScale_);
        handSelectionItem_->setZValue(-1); // behind cards just in case.
    }

    // Reposition all player hands to distribute horizontally across the scene
    for (int i = 0; i < playerHandCards_.size(); i++) {
        repositionHandCards(i, 400); // Longer duration for split animation
    }
}

void CardsView::drawCutCard() {
    // Create cut card item at deck position
    QPixmap cutCardPix = cardSprites_.cutCard();
    cutCardItem_ = scene_->addPixmap(cutCardPix);
    cutCardItem_->setTransformOriginPoint(cutCardItem_->boundingRect().center());
    cutCardItem_->setPos(deckPos_);
    cutCardItem_->setScale(cardScale_);

    // Calculate scaled offsets for positioning
    int scaledDrawOffset = static_cast<int>(DECK_DRAW_OFFSET * cardScale_ * CARD_WIDTH);
    int scaledCardHeight = static_cast<int>(CARD_HEIGHT * cardScale_);
    int scaledCutCardGap = static_cast<int>(CUT_CARD_OFFSET_Y * cardScale_);

    // Final position = deck position + card height + gap
    QPoint finalPosition(deckPos_.x(), deckPos_.y() + scaledCardHeight + scaledCutCardGap);
    QPoint drawPoint(deckPos_.x(), deckPos_.y() + scaledDrawOffset);

    // Animate draw from deck to drawPoint (slightly below deck)
    QVariantAnimation* drawCut = new QVariantAnimation(this);
    drawCut->setDuration(DECK_DRAW_DURATION);
    drawCut->setStartValue(deckPos_);
    drawCut->setEndValue(drawPoint);
    drawCut->setEasingCurve(QEasingCurve::InOutExpo);

    connect(drawCut, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
        cutCardItem_->setPos(v.toPointF());
    });

    // Animate movement to final position below deck
    QVariantAnimation* moveCut = new QVariantAnimation(this);
    moveCut->setDuration(DEAL_TO_HAND_DURATION);
    moveCut->setStartValue(drawPoint);
    moveCut->setEndValue(finalPosition);
    moveCut->setEasingCurve(QEasingCurve::InOutExpo);

    connect(moveCut, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
        cutCardItem_->setPos(v.toPointF());
    });

    // Connect animations in sequence
    connect(drawCut, &QVariantAnimation::finished, this, [moveCut] {
        moveCut->start(QAbstractAnimation::DeleteWhenStopped);
    });

    drawCut->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardsView::updateHandSelectionPosition() {
    if (!hasSplit_ || !handSelectionItem_) {
        return;
    }

    if (currentHandIndex_ < 0 || currentHandIndex_ >= playerHandCards_.size() ||
        playerHandCards_[currentHandIndex_].isEmpty()) {
        handSelectionItem_->setVisible(false);
        return;
    }

    handSelectionItem_->setVisible(true);

    int totalHands = playerHandCards_.size();
    QVector<int> handBasePositions = calculateHandBaseXPositions(totalHands);
    int handBaseX = handBasePositions[currentHandIndex_];

    QVector<int> relativePositions =
        calculateRelativeCardPositions(playerHandCards_[currentHandIndex_].size());

    int handCenterX = handBaseX + relativePositions[relativePositions.size() / 2];
    int playerY = getPlayerHandY();

    // Place the selection under the hand
    int selectionY = playerY + CARD_HEIGHT * 0.85 * cardScale_; // tweak offset?

    handSelectionItem_->setPos(
        handCenterX - handSelectionItem_->boundingRect().width() / 2,
        selectionY - handSelectionItem_->boundingRect().height() / 2
        );
}

void CardsView::setCurrentHandIndex(int index) {
    currentHandIndex_ = index;
    updateHandSelectionPosition();
}

void CardsView::setHasSplit(bool newState){
    hasSplit_ = newState;
}


void CardsView::cleanUp() {
    // Clear the scene
    scene_->clear();

    // Clear card tracking structures
    playerHandCards_.clear();
    dealerHandCards_.clear();

    // Reset pointers (must be done after scene_->clear() to avoid dangling pointers)
    deckItem_ = nullptr;
    cutCardItem_ = nullptr;
    holeCardItem_ = nullptr;

    // Recreate deck at current position
    updateDeckPosition();
    deckItem_ = scene_->addPixmap(cardSprites_.back());
    deckItem_->setTransformOriginPoint(deckItem_->boundingRect().center());
    deckItem_->setPos(deckPos_);
    deckItem_->setScale(cardScale_);
}

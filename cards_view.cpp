#include "cards_view.h"
#include <QResizeEvent>

CardsView::CardsView(QWidget *parent)
    : QWidget(parent), cardSprites_(":/images/cards.png", 2.0),
    cutCardItem_(nullptr), holeCardItem_(nullptr),
    holeCard_(Card::Rank::Cut, Card::Suit::Cut), // Card doesn't matter
    cardScale_(1.0)
{
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
    deckItem_->setPos(deckPos_);

    // Calculate and apply initial card scale
    cardScale_ = calculateCardScale();
    if (deckItem_) {
        deckItem_->setScale(cardScale_);
    }
}

int CardsView::getDealerHandY() const
{
    return static_cast<int>(scene_->sceneRect().height() * DEALER_Y_PERCENT);
}

int CardsView::getPlayerHandY() const
{
    return static_cast<int>(scene_->sceneRect().height() * PLAYER_Y_PERCENT);
}

float CardsView::calculateCardScale() const
{
    // Reference dimensions (what card sizes are designed for)
    // TODO: design a cleaner implementation that doesn't need arbitrary reference dimensions
    const float refWidth = 1600.0;
    const float refHeight = 900.0;

    // Calculate scale based on current scene size
    float scaleX = scene_->sceneRect().width() / refWidth;
    float scaleY = scene_->sceneRect().height() / refHeight;

    // Use minimum to ensure cards fit in both dimensions
    return qMin(scaleX, scaleY);
}

void CardsView::scaleAllCards()
{
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

void CardsView::updateDeckPosition()
{
    // Deck positioned at top-right with margins
    int sceneWidth = static_cast<int>(scene_->sceneRect().width());
    deckPos_ = QPoint(sceneWidth - DECK_MARGIN_RIGHT, DECK_Y_POS);

    // Update deck item position if it exists
    if (deckItem_) {
        deckItem_->setPos(deckPos_);
    }

    // Update cut card position if it exists
    if (cutCardItem_) {
        cutCardItem_->setPos(deckPos_.x(), deckPos_.y() + CUT_CARD_OFFSET_Y);
    }
}

QVector<int> CardsView::calculateCenteredPositions(int numCards) const
{
    if (numCards == 0) return QVector<int>();

    // Use scaled card dimensions
    int scaledCardWidth = static_cast<int>(CARD_WIDTH * cardScale_);

    int cardGap = scaledCardWidth * CARD_GAP;

    int totalWidth = (numCards * scaledCardWidth) + ((numCards - 1) * cardGap);
    int sceneWidth = static_cast<int>(scene_->sceneRect().width());
    int startX = (sceneWidth - totalWidth) / 2;

    QVector<int> positions;
    for (int i = 0; i < numCards; i++) {
        positions.append(startX + (i * (scaledCardWidth + cardGap)));
    }

    return positions;
}

QVector<int> CardsView::calculateHandBaseXPositions(int totalHands) const
{
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

QVector<int> CardsView::calculateRelativeCardPositions(int numCards) const
{
    if (numCards == 0) return QVector<int>();

    // Use scaled card dimensions
    int scaledCardWidth = static_cast<int>(CARD_WIDTH * cardScale_);
    int scaledGap = static_cast<int>(CARD_GAP * cardScale_);

    // Total width of all cards plus gaps
    int totalWidth = (numCards * scaledCardWidth) + ((numCards - 1) * scaledGap);

    // Starting offset from hand center (negative = left of center)
    int startOffset = -totalWidth / 2;

    QVector<int> relativePositions;
    for (int i = 0; i < numCards; i++) {
        relativePositions.append(startOffset + (i * (scaledCardWidth + scaledGap)));
    }

    return relativePositions;
}

void CardsView::resizeEvent(QResizeEvent* event)
{
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

    // Fit view to scene
    view_->fitInView(scene_->sceneRect(), Qt::IgnoreAspectRatio);
}

void CardsView::repositionAllHands()
{
    // Reposition dealer hand (centered at dynamic Y position)
    if (!dealerHandCards_.isEmpty()) {
        repositionHandCards(-1, 0); // Reposition instantly
    }

    // Reposition all player hands (distributed horizontally at dynamic Y position)
    for (int i = 0; i < playerHandCards_.size(); i++) {
        if (!playerHandCards_[i].isEmpty()) {
            repositionHandCards(i, 0); // Reposition instantly
        }
    }
}

void CardsView::repositionHandCards(int handIndex, int duration)
{
    QVector<QGraphicsPixmapItem*> cards;

    if (handIndex == -1) {
        // Dealer hand - centered
        cards = dealerHandCards_;
        if (cards.isEmpty()) return;

        QVector<int> newXPositions = calculateCenteredPositions(cards.size());
        int dealerY = getDealerHandY();

        for (int i = 0; i < cards.size(); i++) {
            QGraphicsPixmapItem* card = cards[i];
            QPointF currentPos = card->pos();
            QPointF newPos(newXPositions[i], dealerY);

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
        QVector<int> relativePositions = calculateRelativeCardPositions(cards.size());
        int playerY = getPlayerHandY();

        // Animate each card
        for (int i = 0; i < cards.size(); i++) {
            QGraphicsPixmapItem* card = cards[i];
            QPointF currentPos = card->pos();

            // Combine hand base position with card offset
            int finalX = handBaseX + relativePositions[i];
            QPointF newPos(finalX, playerY);

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

void CardsView::flipCard(QGraphicsPixmapItem* item, const Card& card)
{
    // Set transform origin to center for proper flipping effect
    item->setTransformOriginPoint(item->boundingRect().center());

    // Animate shrinking card
    QVariantAnimation* shrink = new QVariantAnimation(this);
    shrink->setDuration(FLIP_DURATION);
    shrink->setStartValue(1.0);
    shrink->setEndValue(0.0);

    connect(shrink, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
        item->setScale(v.toReal());
    });

    // Replace card back with front, animate growing card
    connect(shrink, &QVariantAnimation::finished, this, [this, item, card]() {
        item->setPixmap(cardSprites_.faceFor(card));

        QVariantAnimation* grow = new QVariantAnimation(this);
        grow->setDuration(FLIP_DURATION);
        grow->setStartValue(0.0);
        grow->setEndValue(1.0);

        connect(grow, &QVariantAnimation::valueChanged, this, [item](const QVariant& v) {
            item->setScale(v.toReal());
        });

        grow->start(QAbstractAnimation::DeleteWhenStopped);
    });

    shrink->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardsView::dealPlayerCard(const Card& card, int handIndex, bool isLastCard)
{
    // Create card item at deck position
    QPixmap backPix = cardSprites_.back();
    QGraphicsPixmapItem* item = scene_->addPixmap(backPix);
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
    playerHandCards_[handIndex].append(item);

    // Calculate final position
    int numCards = playerHandCards_[handIndex].size();
    int totalHands = playerHandCards_.size();

    QVector<int> handBasePositions = calculateHandBaseXPositions(totalHands);
    int handBaseX = handBasePositions[handIndex];

    QVector<int> relativePositions = calculateRelativeCardPositions(numCards);
    int finalX = handBaseX + relativePositions.last();
    int playerY = getPlayerHandY();

    QPoint handPosition(finalX, playerY);
    QPoint drawPoint(deckPos_.x(), deckPos_.y() + DECK_DRAW_OFFSET);

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

    // On completion: flip card and reposition hand (simultaneously)
    connect(dealCard, &QVariantAnimation::finished, this, [this, item, card, handIndex]() {
        flipCard(item, card);
        // Reposition happens simultaneously with the last part of dealing
        repositionHandCards(handIndex, DEAL_TO_HAND_DURATION);
    });

    drawCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardsView::dealDealerCard(const Card& card)
{
    // Create card item at deck position
    QPixmap backPix = cardSprites_.back();
    QGraphicsPixmapItem* item = scene_->addPixmap(backPix);
    item->setPos(deckPos_);
    item->setScale(cardScale_);

    // Add to dealer hand tracking
    dealerHandCards_.append(item);

    // Calculate final centered position for dealer hand
    int numCards = dealerHandCards_.size();
    QVector<int> xPositions = calculateCenteredPositions(numCards);
    int dealerY = getDealerHandY();

    QPoint handPosition(xPositions.last(), dealerY);
    QPoint drawPoint(deckPos_.x(), deckPos_.y() + DECK_DRAW_OFFSET);

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
        // Reposition dealer cards to maintain centering
        repositionHandCards(-1, DEAL_TO_HAND_DURATION);
    });

    drawCard->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardsView::flipDealerHoleCard()
{
    if (holeCardItem_) {
        flipCard(holeCardItem_, holeCard_);
    }
}

void CardsView::handleHandSplit(int handIndex)
{
    if (handIndex >= playerHandCards_.size() || playerHandCards_[handIndex].size() < 2) {
        return; // Invalid split
    }

    // Insert a new empty hand after the hand being split
    playerHandCards_.insert(handIndex + 1, QVector<QGraphicsPixmapItem*>());

    // Move the second card from the hand being split into the new hand
    QGraphicsPixmapItem* splitCardItem = playerHandCards_[handIndex].takeAt(1);
    playerHandCards_[handIndex + 1].append(splitCardItem);

    // Reposition all player hands to distribute horizontally across the scene
    for (int i = 0; i < playerHandCards_.size(); i++) {
        repositionHandCards(i, 400); // Longer duration for split animation
    }
}

void CardsView::drawCutCard()
{
    // Create cut card item at deck position
    QPixmap cutCardPix = cardSprites_.cutCard();
    cutCardItem_ = scene_->addPixmap(cutCardPix);
    cutCardItem_->setPos(deckPos_);
    cutCardItem_->setScale(cardScale_);

    QPoint finalPosition(deckPos_.x(), deckPos_.y() + CUT_CARD_OFFSET_Y);
    QPoint drawPoint(deckPos_.x(), deckPos_.y() + DECK_DRAW_OFFSET);

    // Animate draw from deck to drawPoint (slightly below deck) to display a
    QVariantAnimation* drawCut = new QVariantAnimation(this);
    drawCut->setDuration(DECK_DRAW_DURATION);
    drawCut->setStartValue(deckPos_);
    drawCut->setEndValue(drawPoint);

    connect(drawCut, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
        cutCardItem_->setPos(v.toPointF());
    });

    // Animate movement to final position below deck
    QVariantAnimation* moveCut = new QVariantAnimation(this);
    moveCut->setDuration(DEAL_TO_HAND_DURATION);
    moveCut->setStartValue(drawPoint);
    moveCut->setEndValue(finalPosition);

    connect(moveCut, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
        cutCardItem_->setPos(v.toPointF());
    });

    // Connect animations in sequence
    connect(drawCut, &QVariantAnimation::finished, this, [moveCut] {
        moveCut->start(QAbstractAnimation::DeleteWhenStopped);
    });

    drawCut->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardsView::cleanUp()
{
    // Clear the scene
    scene_->clear();

    // Clear card tracking structures
    playerHandCards_.clear();
    dealerHandCards_.clear();

    // Reset pointers
    cutCardItem_ = nullptr;
    holeCardItem_ = nullptr;

    // Recreate deck at current position
    updateDeckPosition(); // Recalculate position based on current scene size
    deckItem_ = scene_->addPixmap(cardSprites_.back());
    deckItem_->setPos(deckPos_);
    deckItem_->setScale(cardScale_);
}

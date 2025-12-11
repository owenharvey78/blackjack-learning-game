#ifndef CARDS_VIEW_H
#define CARDS_VIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVariantAnimation>
#include <QVector>
#include <QPoint>
#include "card.h"
#include "card_sprites.h"

/// @brief A specialized widget for rendering and animating blackjack cards.
/// Uses an internal QGraphicsView/QGraphicsScene with dynamic resizing.
/// Scene dimensions match widget dimensions for optimal space usage.
class CardsView : public QWidget
{
    Q_OBJECT

public:
    /// @brief Creates a new CardsView object.
    /// @param parent The parent widget of this CardsView
    explicit CardsView(QWidget* parent = nullptr);

    /// @brief Animates dealing a card to a player hand.
    /// @param card The card to deal.
    /// @param handIndex The index of the hand receiving the card.
    /// @param isLastCard True if this card should be dealt sideways (doubled/split aces).
    void dealPlayerCard(const Card& card, int handIndex, bool isLastCard);

    /// @brief Animates dealing a card to the dealer.
    /// @param card The card to deal.
    void dealDealerCard(const Card& card);

    /// @brief Flips the dealer's hole card (second card).
    void flipDealerHoleCard();

    /// @brief Handles hand split by redistributing cards across the scene.
    /// @param handIndex The index of the hand being split.
    void handleHandSplit(int handIndex);

    /// @brief Animates the cut card being drawn and displayed below the deck.
    void drawCutCard();

    /// @brief Clears all cards and resets the view for a new round.
    void cleanUp();

protected:
    /// @brief Handles resize events - dynamically adjusts scene and repositions all cards.
    /// @param event The resize event.
    void resizeEvent(QResizeEvent* event) override;

private:
    /// @brief Dealer hand Y position as percentage of scene height (0.0 to 1.0).
    static constexpr double DEALER_Y_PERCENT = 0.12;

    /// @brief Player hand(s) Y position as percentage of scene height (0.0 to 1.0).
    static constexpr double PLAYER_Y_PERCENT = 0.78;

    /// @brief Deck X margin from right edge (pixels).
    static constexpr int DECK_RIGHT_MARGIN = 10;

    /// @brief Deck Y margin from top edge (pixels).
    static constexpr int DECK_TOP_MARGIN = 50;

    /// @brief Offset below deck for cut card display (pixels).
    static constexpr int CUT_CARD_OFFSET_Y = 30;

    /// @brief The width of the card sprites in the resource file.
    static constexpr int CARD_WIDTH = 71;

    /// @brief The height of the card sprites in the resource file.
    static constexpr int CARD_HEIGHT = 95;

    /// @brief Horizontal gap between cards in the same hand as a percentage of
    /// the card width (0.0-1.0).
    static constexpr float CARD_GAP = 0.3;

    /// @brief The amount that cards should move down by during the draw
    /// animation as a percentage of the card width (0.0+).
    static constexpr float DECK_DRAW_OFFSET = 1.0;

    /// @brief The duration of the draw animation, in milliseconds.
    static constexpr int DECK_DRAW_DURATION = 150;

    /// @brief The duration of the animation moving cards from the deck to the
    /// dealer's/player's hand after they are drawn.
    static constexpr int DEAL_TO_HAND_DURATION = 300;

    /// @brief The duration of the card flip animation.
    static constexpr int FLIP_DURATION = 150;

    /// @brief Calculates dealer hand Y position based on current scene height.
    /// @return Y position in scene coordinates.
    int getDealerHandY() const;

    /// @brief Calculates player hand Y position based on current scene height.
    /// @return Y position in scene coordinates.
    int getPlayerHandY() const;

    /// @brief Updates deck and cut card positions based on current scene size.
    void updateDeckPosition();

    /// @brief Repositions all hands (dealer and player) based on current scene dimensions.
    void repositionAllHands();

    /// @brief Calculates the appropriate scale factor for cards based on current scene size.
    /// @return Scale factor (1.0 = normal size, <1.0 = smaller, >1.0 = larger).
    float calculateCardScale() const;

    /// @brief Applies current card scale to all existing card items.
    void scaleAllCards();

    /// @brief Flips a card item from back to face with shrink/grow animation.
    /// @param item The graphics item to flip.
    /// @param card The card data (for determining the face pixmap).
    void flipCard(QGraphicsPixmapItem* item, const Card& card);

    /// @brief Calculates base X position for each hand when multiple hands exist.
    /// @param totalHands Total number of player hands.
    /// @return Vector of center X positions for each hand section.
    QVector<int> calculateHandBaseXPositions(int totalHands) const;

    /// @brief Calculates card positions relative to a hand's center, treating
    /// the center as 0 so the hand center can be added to the offsets.
    /// @param numCards Number of cards in the hand.
    /// @return Vector of X offsets from 0.
    QVector<int> calculateRelativeCardPositions(int numCards) const;

    /// @brief Animates repositioning of all cards in a hand to maintain centering.
    /// @param handIndex The hand to reposition (-1 for dealer).
    /// @param duration Animation duration in milliseconds.
    void repositionHandCards(int handIndex, int duration);

    /// @brief The internal graphics view for rendering cards (fills entire widget).
    QGraphicsView* view_;

    /// @brief The graphics scene containing all card items (dynamically resized to match view).
    QGraphicsScene* scene_;

    /// @brief Sprite sheet manager for card rendering.
    CardSprites cardSprites_;

    /// @brief The deck item graphic (positioned at top-right with margins).
    QGraphicsPixmapItem* deckItem_;

    /// @brief The cut card item graphic (displayed below deck when drawn).
    QGraphicsPixmapItem* cutCardItem_;

    /// @brief The current deck position in scene coordinates.
    QPoint deckPos_;

    /// @brief Graphics items for each player hand's cards.
    /// Outer vector = hands, inner vector = cards in each hand.
    QVector<QVector<QGraphicsPixmapItem*>> playerHandCards_;

    /// @brief Graphics items for the dealer's cards.
    QVector<QGraphicsPixmapItem*> dealerHandCards_;

    /// @brief The dealer's hole card item (second card, flipped later).
    QGraphicsPixmapItem* holeCardItem_;

    /// @brief The dealer's hole card data (stored for delayed flip).
    Card holeCard_;

    /// @brief Current scale factor for card items.
    float cardScale_;
};

#endif // CARDS_VIEW_H

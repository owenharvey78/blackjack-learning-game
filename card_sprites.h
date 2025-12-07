#ifndef CARD_SPRITES_H
#define CARD_SPRITES_H

#include <QPixmap>
#include "card.h"

class CardSprites {
public:
    /// @brief Constructor
    /// @param path Holds the filepath for the cards.
    /// @param scale Scale factor for card size (default 1.0 = original size).
    explicit CardSprites(const QString& path, qreal scale = 1.0);

    /// @brief Gets the correct card face for the passed in card.
    /// @param card The passed in card.
    /// @return QPixmap A pixmap of the passed in card grabbed from
    /// the filepath.
    QPixmap faceFor(const Card& card) const;

    /// @brief Gets the card back from the filepath.
    /// @return QPixmap A pixmap of the card back from the filepath.
    QPixmap back() const;

    /// @brief Gets the cut card from the filepath.
    /// @return QPixmap A pixmap of the cut card from the filepath.
    QPixmap cutCard() const;

private:

    /// @brief A pixmap of the filepath (cards.png).
    QPixmap sheet_;

    /// @brief The width of a card from filepath (cards.png).
    int cardWidth_;

    /// @brief The height of a card from filepath (cards.png).
    int cardHeight_;

    /// @brief Scale factor for card rendering.
    qreal scale_;

    /// @brief Gets the rect for the passed in suit and rank.
    /// @param suit The passed in card suit.
    /// @param rank The passed in card rank.
    /// @return QRect A rect that has the correct position of
    /// the passed in suit and rank.
    QRect rectFor(Card::Suit suit, Card::Rank rank) const;
};

#endif // CARD_SPRITES_H

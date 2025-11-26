#include "card_sprites.h"

CardSprites::CardSprites(const QString& path)
    : sheet_(path) {
    cardWidth_  = sheet_.width()  / 14;
    cardHeight_ = sheet_.height() / 4;
}

QRect CardSprites::rectFor(Card::Suit suit, Card::Rank rank) const {
    int row = 0;
    switch (suit) {
        case Card::Suit::Clubs:
            row = 0;
            break;
        case Card::Suit::Diamonds:
            row = 1;
            break;
        case Card::Suit::Hearts:
            row = 2;
            break;
        case Card::Suit::Spades:
            row = 3;
            break;
        case Card::Suit::Cut:
            row = 0; // Using clubs row for cut.
            break;
    }

    int col = 0;
    switch (rank) {
        case Card::Rank::Ace:
            col = 0;
            break;
        case Card::Rank::Two:
            col = 1;
            break;
        case Card::Rank::Three:
            col = 2;
            break;
        case Card::Rank::Four:
            col = 3;
            break;
        case Card::Rank::Five:
            col = 4;
            break;
        case Card::Rank::Six:
            col = 5;
            break;
        case Card::Rank::Seven:
            col = 6;
            break;
        case Card::Rank::Eight:
            col = 7;
            break;
        case Card::Rank::Nine:
            col = 8;
            break;
        case Card::Rank::Ten:
            col = 9;
            break;
        case Card::Rank::Jack:
            col = 10;
            break;
        case Card::Rank::Queen:
            col = 11;
            break;
        case Card::Rank::King:
            col = 12;
            break;
        case Card::Rank::Cut:
            col = 13;
            break;
    }

    return QRect(col * cardWidth_, row * cardHeight_, cardWidth_, cardHeight_);
}

QPixmap CardSprites::faceFor(const Card& card) const {
    return sheet_.copy(rectFor(card.suit, card.rank));
}

QPixmap CardSprites::back() const {

    // back is in last column, diamond row
    QRect r(13 * cardWidth_, 1 * cardHeight_, cardWidth_, cardHeight_);
    return sheet_.copy(r);
}

QPixmap CardSprites::cutCard() const {

    // cut card in last column, e.g. hearts row
    QRect r(13 * cardWidth_, cardHeight_, cardWidth_, cardHeight_);
    return sheet_.copy(r);
}

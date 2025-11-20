#ifndef CARD_H
#define CARD_H

#include <QObject>

class Card : public QObject
{
    Q_OBJECT
public:
    /// @brief The possible suits for a playing card. "Cut" represents the cut card,
    /// which tells the dealer when to shuffle the shoe.
    enum class Suit {
        Spades,
        Hearts,
        Clubs,
        Diamonds,
        Cut
    };

    /// @brief The possible ranks for a playing card. "Cut" describes the cut card,
    /// which tells the dealer when to shuffle the shoe.
    enum class Rank {
        Cut,
        Ace,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King
    };

    /// @brief Creates a new Card object.
    /// @param rank The rank of the card.
    /// @param suit The suit of the card.
    /// @param parent The parent of this Card object.
    explicit Card(Rank rank, Suit suit, QObject *parent = nullptr);

    /// @brief Gets the value of this card according to the Hi-Lo card counting system.
    /// @return 1 if this card's rank is 2-6, 0 if this card's rank is 7-9, or -1 if this
    /// card's rank is jack-ace.
    int getValue() const;

private:
    /// @brief The card's rank.
    const Rank rank_;

    /// @brief The card's suit.
    const Suit suit_;
};

#endif // CARD_H

#ifndef CARD_H
#define CARD_H

struct Card {
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
    explicit Card(Rank rank, Suit suit);

    /// @brief Gets the value of this card according to the Hi-Lo card counting system.
    /// @return 1 if this card's rank is 2-6, -1 if this card's rank is jack-ace, or 0
    /// otherwise.
    int getValue() const;

private:
    /// @brief The card's rank.
    const Rank rank_;

    /// @brief The card's suit.
    const Suit suit_;
};

#endif // CARD_H

#ifndef SHOE_H
#define SHOE_H

#include <QObject>
#include <QVector>
#include <QRandomGenerator>
#include "card.h"

/// @brief A class representing the shoe that holds the cards to be drawn.
class Shoe : public QObject
{
    Q_OBJECT
public:
    /// @brief Constructs a new Shoe.
    /// @param decks The number of full decks of cards in the shoe (default 6).
    /// @param penetration The location of the cut card relative to the rest of the
    /// shoe.
    /// @param parent The parent object of this Shoe object.
    Shoe(int decks = 6, float penetration = 0.2, QObject* parent = nullptr);

    /// @brief Draws a card from the shoe.
    /// @return The card drawn from the shoe.
    Card* draw();

    /// @brief Gets the number of cards remaining in the shoe.
    /// @return The number of cards remaining in the shoe.
    size_t getSize() const;

    /// @brief Resets the shoe, giving it the number of decks and penetration provided at
    /// construction and reordering all cards.
    void shuffle();

private:
    /// @brief The number of decks in the shoe before any cards are drawn. Used for shuffling
    /// the deck.
    const int decks_;

    /// @brief The location of the cut card relative to the rest of the shoe. Used for shuffling
    /// the deck.
    const float penetration_;

    /// @brief The cards in the shoe.
    QVector<Card*> cards_;

    /// @brief The random number generator for shuffling cards.
    QRandomGenerator rng_;

    /// @brief Randomizes the order of all cards in the given list.
    /// @param list The list of cards to randomize.
    /// @param rng The random number generator for randomizing the cards.
    static void randomize(QVector<Card*>& list, QRandomGenerator& rng);

    /// @brief Adds a single deck to the shoe's cards. The cards are sorted in order of rank,
    /// then in order of suit.
    void addDeck();
};

#endif // SHOE_H

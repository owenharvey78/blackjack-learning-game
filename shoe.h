#ifndef SHOE_H
#define SHOE_H

#include <QObject>

/// @brief A class representing the shoe that holds the cards to be drawn.
class Shoe : public QObject
{
    Q_OBJECT
public:
    Shoe();
};

#endif // SHOE_H

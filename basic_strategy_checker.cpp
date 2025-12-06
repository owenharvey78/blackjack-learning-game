#include "basic_strategy_checker.h"
#include "card.h"
#include <QVector>
#include <tuple>

using std::tuple;

BasicStrategyChecker::BasicStrategyChecker(bool dealerHitsSoft17) :
    dealerHitsSoft17_(dealerHitsSoft17)
{}

int BasicStrategyChecker::getUpcardIndex(int upcardValue) {
    return upcardValue - 2;
}

int BasicStrategyChecker::getHardTotalsRowIndex(int handTotal) {
    return handTotal - 4;
}

int BasicStrategyChecker::getSoftTotalsRowIndex(int softTotal) {
    return softTotal - 12;
}

tuple<int, bool> BasicStrategyChecker::getHandTotal(const QVector<Card>& hand) {
    int total = 0;
    int aceCount = 0;
    for (auto const& card : hand) {
        total += card.getBlackjackValue();
        if (card.rank == Card::Rank::Ace)
            aceCount++;
    }

    while (total > 21) {
        // Total will only be greater than 21 as long as there is at least one
        // ace currently being counted as 11 (if not, the hand was invalid when
        // the function was called
        total -= 10;
        aceCount--;
    }

    return { total, aceCount > 0 };
}

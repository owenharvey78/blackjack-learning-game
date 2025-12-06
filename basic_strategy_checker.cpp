#include "basic_strategy_checker.h"
#include "card.h"
#include <QVector>
#include <tuple>
#include <cassert>

using std::tuple;

BasicStrategyChecker::BasicStrategyChecker(bool dealerHitsSoft17) :
    dealerHitsSoft17_(dealerHitsSoft17)
{}

int BasicStrategyChecker::getUpcardIndex(Card dealerUpcard) {
    return dealerUpcard.getBlackjackValue() - 2;
}

int BasicStrategyChecker::getHardTotalsRowIndex(int handTotal) {
    return handTotal - 4;
}

int BasicStrategyChecker::getSoftTotalsRowIndex(int softTotal) {
    return softTotal - 12;
}

int BasicStrategyChecker::getSplittingRowIndex(Card card) {
    return card.getBlackjackValue() - 2;
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
        // the function was called)
        total -= 10;
        aceCount--;
    }

    return { total, aceCount > 0 };
}

bool BasicStrategyChecker::canSplit(const QVector<Card>& hand) {
    return hand.size() == 2 && hand[0].getBlackjackValue() == hand[1].getBlackjackValue();
}

BasicStrategyChecker::PlayerAction BasicStrategyChecker::getBestMove(const QVector<Card>& hand, Card dealerUpcard) {
    if (dealerHitsSoft17_) {
        // Use H17 strategy
        if (canSplit(hand))
            return H17_SPLITTING[getSplittingRowIndex(hand[0])][getUpcardIndex(dealerUpcard)];

        const auto [handTotal, isSoftTotal] = getHandTotal(hand);

        if (isSoftTotal)
            return H17_SOFT_TOTALS[getSoftTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
        else
            return H17_HARD_TOTALS[getHardTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
    }
    else {
        // Use S17 strategy
        if (canSplit(hand))
            return S17_SPLITTING[getSplittingRowIndex(hand[0])][getUpcardIndex(dealerUpcard)];

        const auto [handTotal, isSoftTotal] = getHandTotal(hand);

        if (isSoftTotal)
            return S17_SOFT_TOTALS[getSoftTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
        else
            return S17_HARD_TOTALS[getHardTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
    }
}

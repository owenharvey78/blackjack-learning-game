#include "basic_strategy_checker.h"
#include "blackjack_game.h"
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

bool BasicStrategyChecker::canSplit(const QVector<Card>& hand) {
    return hand.size() == 2 && hand[0].getBlackjackValue() == hand[1].getBlackjackValue();
}

BasicStrategyChecker::PlayerAction BasicStrategyChecker::getBestMove(const QVector<Card>& hand, Card dealerUpcard) const {
    if (dealerHitsSoft17_) {
        // Use H17 strategy
        if (canSplit(hand))
            return H17_SPLITTING[getSplittingRowIndex(hand[0])][getUpcardIndex(dealerUpcard)];

        int handTotal = BlackjackGame::getHandValue(const_cast<QVector<Card>&>(hand));
        bool isSoftTotal = BlackjackGame::isSoftHand(hand);

        if (isSoftTotal)
            return H17_SOFT_TOTALS[getSoftTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
        else
            return H17_HARD_TOTALS[getHardTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
    }
    else {
        // Use S17 strategy
        if (canSplit(hand))
            return S17_SPLITTING[getSplittingRowIndex(hand[0])][getUpcardIndex(dealerUpcard)];

        int handTotal = BlackjackGame::getHandValue(const_cast<QVector<Card>&>(hand));
        bool isSoftTotal = BlackjackGame::isSoftHand(hand);

        if (isSoftTotal)
            return S17_SOFT_TOTALS[getSoftTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
        else
            return S17_HARD_TOTALS[getHardTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
    }
}

BasicStrategyChecker::PlayerAction BasicStrategyChecker::getSecondBestMove(const QVector<Card>& hand, Card dealerUpcard) const {
    BasicStrategyChecker::PlayerAction firstResult = getBestMove(hand, dealerUpcard);
    int handTotal = BlackjackGame::getHandValue(const_cast<QVector<Card>&>(hand));
    bool isSoftTotal = BlackjackGame::isSoftHand(hand);

    if (dealerHitsSoft17_) {
        // Use H17 strategy

        // Special case: [8, 8] against dealer's ace. The player should surrender,
        // but if they can't surrender, they should split. We must program this
        // manually.
        if (hand.size() == 2 && hand[0].rank == Card::Rank::Eight &&
            hand[1].rank == Card::Rank::Eight && dealerUpcard.rank == Card::Rank::Ace)
            return PlayerAction::Split;

        if (firstResult == BasicStrategyChecker::PlayerAction::Split ||
            firstResult == BasicStrategyChecker::PlayerAction::SplitIfDas) {
            // If the best move was to split or surrender, but the player can't do that,
            // then default to the hard/soft total tables
            if (isSoftTotal)
                return H17_SOFT_TOTALS[getSoftTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
            else
                return H17_HARD_TOTALS[getHardTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
        }
        else if (firstResult == BasicStrategyChecker::PlayerAction::Double) {
            // If the best move was to double, the player should usually hit; but there
            // are a few cases where they should stand. There are few enough of these
            // cases that it's fine to check them all in here.
            if (isSoftTotal && ((handTotal == 18 && dealerUpcard.getBlackjackValue() <= 6) ||
                                ( handTotal == 19 && dealerUpcard.getBlackjackValue() == 6)))
                return PlayerAction::Stand;
            else
                return PlayerAction::Hit;
        }
        else if (firstResult == BasicStrategyChecker::PlayerAction::Surrender) {
            // If best move was to surrender, but the player cannot do so, then hit
            // unless the hand total is 17 (the only possibilities are 15, 16, and
            // 17) against the dealer's ace. Again, it's fine to hard-code these
            // rare exceptions instead of using an entire new table.
            if (handTotal == 17 && dealerUpcard.rank == Card::Rank::Ace)
                return PlayerAction::Stand;
            else
                return PlayerAction::Hit;
        }
    }
    else {
        if (firstResult == PlayerAction::Split || firstResult == PlayerAction::SplitIfDas) {
            // If the best move was to split or surrender, but the player can't do that,
            // then default to the hard/soft total tables
            if (isSoftTotal)
                return S17_SOFT_TOTALS[getSoftTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
            else
                return S17_HARD_TOTALS[getHardTotalsRowIndex(handTotal)][getUpcardIndex(dealerUpcard)]; // NOLINT(clang-analyzer-security.ArrayBound)
        }
        else if (firstResult == PlayerAction::Double) {
            // If the best move was to double, the player should usually hit; but there
            // are a few cases where they should stand. There are few enough of these
            // cases that it's fine to check them all in here.
            if (isSoftTotal && handTotal == 18 && dealerUpcard.getBlackjackValue() >= 3 &&
                dealerUpcard.getBlackjackValue() <= 6)
                return PlayerAction::Stand;
            else
                return PlayerAction::Hit;
        }
        else if (firstResult == PlayerAction::Surrender) {
            // If best move was to surrender, but the player cannot do so, then hit
            // unless the hand total is 17 (the only possibilities are 15, 16, and
            // 17) against the dealer's ace. Again, it's fine to hard-code these
            // rare exceptions instead of using an entire new table.
            if (handTotal == 17 && dealerUpcard.rank == Card::Rank::Ace)
                return PlayerAction::Stand;
            else
                return PlayerAction::Hit;
        }
    }

    // If firstResult was either PlayerAction::Hit or PlayerAction::Stand,
    // return that, because the player will always be able to do that
    return firstResult;
}

BasicStrategyChecker::PlayerAction BasicStrategyChecker::getThirdBestMove(const QVector<Card>& hand, Card dealerUpcard) const {
    // Check the one special case ([8, 8] against dealer's ace in H17)
    if (dealerHitsSoft17_ && hand.size() == 2 && hand[0].rank == Card::Rank::Eight &&
        hand[1].rank == Card::Rank::Eight && dealerUpcard.rank == Card::Rank::Ace)
        return PlayerAction::Hit;

    // Otherwise the result is same as getSecondBestMove
    return getSecondBestMove(hand, dealerUpcard);
}

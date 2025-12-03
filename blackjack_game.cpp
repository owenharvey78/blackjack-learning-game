#include "blackjack_game.h"
#include "shoe.h"
#include "card.h"

BlackjackGame::BlackjackGame(QObject *parent) : QObject{parent},
    rules_(), shoe_(new Shoe(rules_.numDecks, 0.2, this)),
    balance_(1000), needsShuffling_(true), hasRoundStarted_(false),
    currentHandIndex_(0)
{ }

void BlackjackGame::setRuleset(Ruleset rules) {
    rules_ = rules;
}

// Game start and Animation

void BlackjackGame::beginRound(int betAmount) {
    balance_ -= betAmount;
    emit betPlaced(betAmount, balance_);
    dealNewHand();
}

void BlackjackGame::dealNewHand() {
    // Check shuffling status.
    if(needsShuffling_) {
        shoe_->shuffle();
        needsShuffling_ = false;
    }

    // Reset necessary elements.
    playerHands_.clear();
    playerHands_.append(QVector<Card>());
    dealerHand_.clear();
    currentHandIndex_ = 0;

    // Call the animated dealer
    dealCards();
}

void BlackjackGame::dealCards() {
    int delay = 0;
    const int step = 500;  // 500ms between cards

    for (int i = 0; i < 2; ++i) {
        // Deal Player Card
        QTimer::singleShot(delay, this, [this]() {
            dealPlayerCard(0, false);
        });

        delay += step;

        // Deal Dealer Card
        QTimer::singleShot(delay, this, &BlackjackGame::dealDealerCard);
        delay += step;
    }
}

void BlackjackGame::dealPlayerCard(int handIndex, bool isLastCard) {
    Card c = drawCardFromShoe();
    // Safety check in case game closed
    if (playerHands_.isEmpty()) return;

    playerHands_[handIndex].append(c);
    emit playerCardDealt(c, handIndex, isLastCard);
}

void BlackjackGame::dealDealerCard() {
    Card c = drawCardFromShoe();
    dealerHand_.append(c);
    emit dealerCardDealt(c);

    // If this is the last card (2nd round, dealer), check for BJ
    if (dealerHand_.size() == 2) {
        // TODO: add a signal (and a slot in GameWidget) to show a small animation
        // with the dealer checking their other card if their upcard is an ace or
        // 10-valued

        // Check Immediate Blackjack after animation finishes
        bool playerHasBJ = isBlackJack(playerHands_[0]);
        bool dealerHasBJ = isBlackJack(dealerHand_);

        if (playerHasBJ || dealerHasBJ) {
            checkCardsAndRound(0, determineWinner(playerHands_[0], dealerHand_));
        } else {
            // Player's turn - check if double/split allowed
            bool canDbl = canDouble(playerHands_[0], 0);
            bool canSpl = canSplit(playerHands_[0], 0);
            emit playerTurn(0, canDbl, canSpl);
        }
    }
}

// Game logic.

int BlackjackGame::getHandValue(QVector<Card>& hand) {
    int value = 0;
    int aceCount = 0;

    for (const auto& card : hand) {
        value += card.getBlackjackValue();
        if (card.rank == Card::Rank::Ace) {
            aceCount++;
        }
    }

    // Account for aces
    while (value > 21 && aceCount > 0) {
        value -= 10; // Count ace as 1 instead of 11 to not bust.
        aceCount--; // Player still has ace but it is locked as a 1 now.
    }

    return value;
}

bool BlackjackGame::isBust(QVector<Card>& hand) {
    return getHandValue(hand) > 21;
}

bool BlackjackGame::isBlackJack(const QVector<Card>& hand) {
    return hand.size() == 2 && getHandValue(const_cast<QVector<Card>&>(hand)) == 21;
}

bool BlackjackGame::is21(const QVector<Card> &hand) {
    return getHandValue(const_cast<QVector<Card>&>(hand)) == 21;
}

bool BlackjackGame::isSoftHand(const QVector<Card>& hand) {
    int value = 0;
    bool hasAce = false;

    for (const auto& card : hand) {
        value += card.getBlackjackValue();
        if (card.rank == Card::Rank::Ace) {
            hasAce = true;
        }
    }

    return hasAce && (value + 10) <= 21;
}

bool BlackjackGame::canDouble(const QVector<Card>& hand, int currentSplitCount) const {
    return hand.size() == 2 && (rules_.doubleAfterSplit || currentSplitCount == 0);
}

bool BlackjackGame::canSurrender(const QVector<Card>& hand) const {
    return rules_.surrenderAllowed && hand.size() == 2;
}

bool BlackjackGame::canSplit(const QVector<Card>& hand, int currentSplitCount) const {
    if (hand.size() != 2) {
        return false;
    }
    if (hand[0].getBlackjackValue() != hand[1].getBlackjackValue()) {
        return false;
    }
    if (hand[0].rank == Card::Rank::Ace && !rules_.resplitAces && currentSplitCount > 0) {
        return false;
    }
    return true;
}

BlackjackGame::GameResult BlackjackGame::determineWinner(QVector<Card>& playerHand, QVector<Card>& dealerHand) {
    int playerValue = getHandValue(playerHand);
    int dealerValue = getHandValue(dealerHand);

    // Check Busts
    if (playerValue > 21) {
        return GameResult::PlayerBust;
    }
    if (dealerValue > 21) {
        return GameResult::DealerBust;
    }

    // Check Blackjacks (BJ beats non-BJ 21)
    bool pBJ = isBlackJack(playerHand);
    bool dBJ = isBlackJack(dealerHand);

    if (pBJ && !dBJ) {
        return GameResult::PlayerBlackjack;
    }
    if (dBJ && !pBJ) {
        return GameResult::DealerWin;
    }
    if (pBJ && dBJ) {
        return GameResult::Push;
    }

    // Compare Values
    if (playerValue > dealerValue) {
        return GameResult::PlayerWin;
    }
    if (playerValue < dealerValue) {
        return GameResult::DealerWin;
    }

    // Otherwise a push
    return GameResult::Push;
}

// Game logic methods.

bool BlackjackGame::dealerShouldHit(QVector<Card>& hand) const {
    int value = getHandValue(hand);
    if (value < 17) {
        return true;
    }
    if (value == 17 && rules_.dealerHitsSoft17 && isSoftHand(hand)) {
        return true;
    }
    return false;
}

void BlackjackGame::dealerTurn() {
    emit dealerTurnStarted();
    continueDealerTurn();
}

void BlackjackGame::continueDealerTurn() {
    if (dealerShouldHit(dealerHand_)) {
        dealerHit();
        // Wait for animation delay before continuing
        QTimer::singleShot(1000, this, &BlackjackGame::continueDealerTurn);
    }
    else {
        dealerStand();
    }
}

void BlackjackGame::dealerHit() {
    Card c = drawCardFromShoe();
    dealerHand_.append(c);
    emit dealerCardDealt(c);
}

void BlackjackGame::dealerStand() {
    for (int i = 0; i < playerHands_.size(); i++) {
        GameResult result = determineWinner(playerHands_[i], dealerHand_);
        checkCardsAndRound(i, result);
    }
}

// Player Actions

void BlackjackGame::playerHit() {
    if (currentHandIndex_ >= playerHands_.size()) return;

    dealPlayerCard(currentHandIndex_, false);

    if (isBust(playerHands_[currentHandIndex_])) {
        playerStand();
    }
}

void BlackjackGame::playerDouble() {
    if (currentHandIndex_ >= playerHands_.size()) return;
    if (!canDouble(playerHands_[currentHandIndex_], playerHands_.size() - 1)) return;

    dealPlayerCard(currentHandIndex_, true);

    // Wait a short delay before the dealer's turn
    QTimer::singleShot(500, this, &BlackjackGame::playerStand);
}

void BlackjackGame::playerStand() {
    if (currentHandIndex_ < playerHands_.size() - 1) {
        currentHandIndex_++;
    }
    else {
        dealerTurn();
    }
}

void BlackjackGame::playerSplit() {
    if (!canSplit(playerHands_[currentHandIndex_], playerHands_.size() - 1)) return;

    Card splitCard = playerHands_[currentHandIndex_].takeLast();
    QVector<Card> newHand;
    newHand.append(splitCard);
    playerHands_.insert(currentHandIndex_ + 1, newHand);

    // Deal new cards
    dealPlayerCard(currentHandIndex_, splitCard.rank == Card::Rank::Ace);

    dealPlayerCard(currentHandIndex_ + 1, splitCard.rank == Card::Rank::Ace);

    emit splitHand(playerHands_.size());
}

// Results

void BlackjackGame::checkCardsAndRound(int handIndex, GameResult currentState) {
    int payout = 0;
    // TODO: calculate payout
    emit roundEnded(currentState, payout);
}

Card BlackjackGame::drawCardFromShoe() {
    Card c = shoe_->draw();
    if (c.rank == Card::Rank::Cut) {
        needsShuffling_ = true;
        c = shoe_->draw();
    }
    return c;
}

// TODO

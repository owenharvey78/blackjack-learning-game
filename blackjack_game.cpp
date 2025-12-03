#include "blackjack_game.h"
#include "shoe.h"
#include "card.h"

BlackjackGame::BlackjackGame(QObject *parent) : QObject{parent}{
    rules_ = Ruleset();
    shoe_ = new Shoe(rules_.numDecks, 0.75f, this);
    needsShuffling_ = true;
    hasRoundStarted_ = false;
    playerHands_ = QVector<QVector<Card>>();
    dealerHand_ = QVector<Card>();
    currentHandIndex_ = 0;

    // Add necessary connects here.
}

void BlackjackGame::setRuleset(Ruleset rules) {
    rules_ = rules;
}

// Game start and Animation

void BlackjackGame::gameStart() {
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
            Card c = drawCardFromShoe();
            // Safety check in case game closed
            if (playerHands_.isEmpty()) return;

            playerHands_[0].append(c);
            emit playerCardDealt(c);
        });

        delay += step;

        // Deal Dealer Card
        QTimer::singleShot(delay, this, [this]() {
            Card c = drawCardFromShoe();
            dealerHand_.append(c);
            emit dealerCardDealt(c);

            // If this is the last card (2nd round, dealer), check for BJ
            if (dealerHand_.size() == 2) {
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
        });
        delay += step;
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

    Card c = drawCardFromShoe();
    playerHands_[currentHandIndex_].append(c);
    emit playerCardDealt(c);

    if (isBust(playerHands_[currentHandIndex_])) {
        playerStand();
    }
}

void BlackjackGame::playerDouble() {
    if (currentHandIndex_ >= playerHands_.size()) return;
    if (!canDouble(playerHands_[currentHandIndex_], playerHands_.size() - 1)) return;

    Card c = drawCardFromShoe();
    playerHands_[currentHandIndex_].append(c);
    emit playerCardDealt(c);

    playerStand();
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
    Card newCard1 = drawCardFromShoe();
    playerHands_[currentHandIndex_].append(newCard1);
    emit playerCardDealt(newCard1);

    Card newCard2 = drawCardFromShoe();
    playerHands_[currentHandIndex_ + 1].append(newCard2);
    emit playerCardDealt(newCard2);

    emit splitHand(playerHands_.size());
}

// Results

void BlackjackGame::checkCardsAndRound(int handIndex, GameResult currentState) {
    emit roundEnded(currentState);
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

#include "blackjack_game.h"
#include "shoe.h"
#include "card.h"

BlackjackGame::BlackjackGame(QObject *parent) : QObject{parent},
    rules_(), shoe_(new Shoe(rules_.numDecks, 0.2, this)),
    balance_(1000), currentBetAmount_(0), needsShuffling_(true), hasRoundStarted_(false),
    currentHandIndex_(0), resultHandIndex_(0)
{ }

void BlackjackGame::setRuleset(Ruleset rules) {
    rules_ = rules;
}

// Game start and Animation

void BlackjackGame::beginRound(int betAmount) {
    currentBetAmount_ = betAmount;
    balance_ -= betAmount;
    emit betPlaced(betAmount);
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
    betAmounts_.clear();
    betAmounts_.append(currentBetAmount_);
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
            emit playerTurn(0, canDouble(), canSplit());
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

bool BlackjackGame::canDouble() const {
    return playerHands_[currentHandIndex_].size() == 2 && (rules_.doubleAfterSplit || playerHands_.size() == 1)
        && balance_ >= betAmounts_[currentHandIndex_];
}

bool BlackjackGame::canSurrender() const {
    return rules_.surrenderAllowed && playerHands_[currentHandIndex_].size() == 2;
}

bool BlackjackGame::canSplit() const {
    if (playerHands_[currentHandIndex_].size() != 2) {
        return false;
    }
    if (playerHands_[currentHandIndex_][0].getBlackjackValue() !=
        playerHands_[currentHandIndex_][1].getBlackjackValue()) {
        return false;
    }
    if (playerHands_.size() > 1) {
        if (!rules_.resplit)
            return false;
        if (playerHands_[currentHandIndex_][0].rank == Card::Rank::Ace && !rules_.resplitAces)
            return false;
    }
    if (balance_ < betAmounts_[currentHandIndex_]) {
        return false;
    }
    return true;
}

BlackjackGame::GameResult BlackjackGame::determineWinner(QVector<Card>& playerHand, QVector<Card>& dealerHand) {
    int playerValue = getHandValue(playerHand);
    int dealerValue = getHandValue(dealerHand);

    // Check Busts
    if (playerValue > 21) {
        return GameResult::Lose;
    }
    if (dealerValue > 21) {
        return GameResult::Win;
    }

    // Check Blackjacks if on first hand
    bool pBJ = isBlackJack(playerHand);
    bool dBJ = isBlackJack(dealerHand);

    if (pBJ && !dBJ) {
        return GameResult::Blackjack;
    }
    if (dBJ && !pBJ) {
        return GameResult::Lose;
    }
    if (pBJ && dBJ) {
        return GameResult::Push;
    }

    // Compare Values
    if (playerValue > dealerValue) {
        return GameResult::Win;
    }
    if (playerValue < dealerValue) {
        return GameResult::Lose;
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
    emit dealerTurnStarted();  // Always reveal hole card

    // Only continue drawing if at least one player hand is alive
    if (allHandsBusted()) {
        dealerStand();
    } else {
        continueDealerTurn();
    }
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
    resultHandIndex_ = 0;  // Start with first hand
    processNextHandResult();
}

void BlackjackGame::processNextHandResult() {
    // Check if there are more hands to process
    if (resultHandIndex_ < playerHands_.size()) {
        // Determine result for current hand
        GameResult result = determineWinner(playerHands_[resultHandIndex_], dealerHand_);

        // Calculate payout
        int payout = 0;
        switch (result) {
        case GameResult::Win:
            payout = betAmounts_[resultHandIndex_] * 2;
            break;
        case GameResult::Push:
            payout = betAmounts_[resultHandIndex_];
            break;
        case GameResult::Blackjack:
            payout = static_cast<int>(betAmounts_[resultHandIndex_] * (1 + rules_.blackjackPayout));
            break;
        default:
            // Lose: payout = 0
            break;
        }

        balance_ += payout;
        emit roundEnded(result, payout, resultHandIndex_, playerHands_.size());

        // Move to next hand
        resultHandIndex_++;

        // Schedule processing of next hand after delay (2 seconds)
        QTimer::singleShot(2000, this, &BlackjackGame::processNextHandResult);
    }
    // All hands processed - UI will handle final reset
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
    if (!canDouble()) return;

    dealPlayerCard(currentHandIndex_, true);

    // Wait a short delay before the dealer's turn
    QTimer::singleShot(500, this, &BlackjackGame::playerStand);
}

void BlackjackGame::playerStand() {
    if (currentHandIndex_ < playerHands_.size() - 1) {
        currentHandIndex_++;
        if (getHandValue(playerHands_[currentHandIndex_]) < 21 &&
            (rules_.hitSplitAces ||
            playerHands_[currentHandIndex_][0].rank == Card::Rank::Ace))
            emit playerTurn(currentHandIndex_, canDouble(), canSplit());
    }
    else {
        dealerTurn();
    }
}

void BlackjackGame::playerSplit() {
    if (!canSplit()) return;

    // Update player hands
    Card splitCard = playerHands_[currentHandIndex_].takeLast();
    QVector<Card> newHand;
    newHand.append(splitCard);
    playerHands_.insert(currentHandIndex_ + 1, newHand);

    // Update bet amounts
    betAmounts_.insert(currentHandIndex_ + 1, betAmounts_[currentHandIndex_]);
    balance_ -= betAmounts_[currentHandIndex_];
    emit betPlaced(betAmounts_[currentHandIndex_]);

    // New cards should be last cards of their hands if splitting aces and
    // rules declare no hitting after splitting aces
    bool isLastCard = splitCard.rank == Card::Rank::Ace && !rules_.hitSplitAces;

    emit splitHand(currentHandIndex_);

    // Deal new cards
    dealPlayerCard(currentHandIndex_, isLastCard);
    dealPlayerCard(currentHandIndex_ + 1, isLastCard);

    if (isLastCard) {
        // Stand after a short delay
        QTimer::singleShot(500, this, &BlackjackGame::playerStand);
    } else {
        // Normal split: emit playerTurn for the first split hand
        emit playerTurn(currentHandIndex_, canDouble(), canSplit());
    }
}

// Results

void BlackjackGame::checkCardsAndRound(int handIndex, GameResult currentState) {
    int payout = 0;
    switch (currentState) {
    case GameResult::Win:
        // Give player back the money they bet and the money they made
        payout = betAmounts_[handIndex] * 2;
        break;
    case GameResult::Push:
        // Give player back the money they bet
        payout = betAmounts_[handIndex];
        break;
    case GameResult::Blackjack:
        // Give back bet amount, then give payout based on ruleset
        payout = static_cast<int>(betAmounts_[handIndex] * (1 + rules_.blackjackPayout));
        break;
    default:
        // If player loses, pay nothing
        break;
    }
    balance_ += payout;
    emit roundEnded(currentState, payout, handIndex, playerHands_.size());
}

Card BlackjackGame::drawCardFromShoe() {
    Card c = shoe_->draw();
    if (c.rank == Card::Rank::Cut) {
        needsShuffling_ = true;
        c = shoe_->draw();
    }
    return c;
}

bool BlackjackGame::allHandsBusted() const {
    for (const auto& hand : playerHands_) {
        if (!isBust(const_cast<QVector<Card>&>(hand))) {
            return false;
        }
    }
    return true;
}

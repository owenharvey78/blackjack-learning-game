#include "blackjack_game.h"
#include "shoe.h"
#include "card.h"

BlackjackGame::BlackjackGame(QObject *parent) : QObject{parent},
    rules_(), shoe_(new Shoe(rules_.numDecks, 0.99, this)),
    balance_(1000), currentBetAmount_(0), needsShuffling_(true), hasRoundStarted_(false),
    currentHandIndex_(0), resultHandIndex_(0), strategyChecker_(rules_.dealerHitsSoft17)
{}

void BlackjackGame::setRuleset(Ruleset rules) {
    rules_ = rules;
}

void BlackjackGame::setShuffling(bool needsShuffling) {
    needsShuffling_ = needsShuffling;
}

// Game start and Animation

void BlackjackGame::beginRound(int betAmount) {
    currentBetAmount_ = betAmount;
    balance_ -= betAmount;

    hasRoundStarted_ = true;

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
    runningCount_ = 0;

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

    // Add Hi-Lo Count to running count
    runningCount_ += c.getHiLoValue();

    // Safety check in case game closed
    if (playerHands_.isEmpty()) return;

    playerHands_[handIndex].append(c);
    emit playerCardDealt(c, handIndex, isLastCard);
}

void BlackjackGame::dealDealerCard() {
    Card c = drawCardFromShoe();

    dealerHand_.append(c);
    emit dealerCardDealt(c);

    if(dealerHand_.size() == 1) {
        // Add Hi-Lo Count to running count
        runningCount_ += c.getHiLoValue();
    }

    // If this is the last card (2nd round, dealer), check for BJ
    if (dealerHand_.size() == 2) {
        QTimer::singleShot(600, this, [this]() {
            bool playerHasBJ = isBlackJack(playerHands_[0]);
            bool dealerHasBJ = isBlackJack(dealerHand_);

            if (playerHasBJ || dealerHasBJ) {
                // Now that the animation is done, it is safe to flip the hole card
                emit dealerTurnStarted();
                checkCardsAndRound(0, determineWinner(playerHands_[0], dealerHand_, false));
            }
            else {
                // Player's turn - check if double/split allowed
                emit playerTurn(0, canDouble(), canSplit(), canSurrender());
            }
        });
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

    return hasAce && value <= 21;
}

bool BlackjackGame::canDouble() const {
    return playerHands_[currentHandIndex_].size() == 2 && (rules_.doubleAfterSplit || playerHands_.size() == 1)
        && balance_ >= betAmounts_[currentHandIndex_];
}

bool BlackjackGame::canSurrender() const {
    // Surrender must be allowed in rules
    if (!rules_.surrenderAllowed) return false;

    if (!hasRoundStarted_) return false;

    // Only allow on the initial hand, before any split
    if (playerHands_.size() != 1) return false;
    if (currentHandIndex_ != 0) return false;

    // Only when this hand has exactly two cards (no hits yet)
    if (playerHands_[0].size() != 2) return false;

    return true;
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

BlackjackGame::GameResult BlackjackGame::determineWinner(QVector<Card>& playerHand, QVector<Card>& dealerHand, bool isSplitHand) {
    int playerValue = getHandValue(playerHand);
    int dealerValue = getHandValue(dealerHand);

    // Check Busts
    if (playerValue > 21) {
        return GameResult::Lose;
    }
    if (dealerValue > 21) {
        return GameResult::Win;
    }

    // Check Blackjacks - only count as blackjack if not from a split
    bool pBJ = !isSplitHand && isBlackJack(playerHand);
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
    }
    else {
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
        bool isSplitHand = playerHands_.size() > 1;
        GameResult result = determineWinner(playerHands_[resultHandIndex_], dealerHand_, isSplitHand);

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
    else {
        hasRoundStarted_ = false;
    }
    // All hands processed - UI will handle final reset
}

bool BlackjackGame::isHandBust(QVector<Card> hand) const {
    return isBust(hand);
}

int BlackjackGame::playerHandValue(QVector<Card> hand) const {
    return getHandValue(hand);
}

// Player Actions

void BlackjackGame::playerHit() {
    if (currentHandIndex_ >= playerHands_.size()) return;

    dealPlayerCard(currentHandIndex_, false);

    if (isBust(playerHands_[currentHandIndex_])) {
        playerStand();
        return;
    }

    int value = getHandValue(playerHands_[currentHandIndex_]);

    if (value == 21) {
        playerStand();
        return;
    }
}

void BlackjackGame::playerDouble() {
    if (currentHandIndex_ >= playerHands_.size()) return;
    if (!canDouble()) return;

    dealPlayerCard(currentHandIndex_, true);
    balance_ -= betAmounts_[currentHandIndex_];
    emit betPlaced(betAmounts_[currentHandIndex_]);
    betAmounts_[currentHandIndex_] *= 2;

    // Wait a short delay before the dealer's turn
    QTimer::singleShot(500, this, &BlackjackGame::playerStand);
}

int BlackjackGame::getRunningCount() {
    return runningCount_;
}

float BlackjackGame::getTrueCount(){
    return (float)runningCount_ * 52 / (shoe_->getSize() - 1); // Need to subtract 1 to account for cut card
}

void BlackjackGame::playerSurrender() {
    // If surrender is not allowed at this moment, ignore the action.
    if (!canSurrender()) return;

    int handIndex = currentHandIndex_;
    int bet = betAmounts_[handIndex];

    // Late surrender: player gets half the bet back, loses the hand.
    int refund = bet / 2;

    balance_ += refund;

    hasRoundStarted_ = false;

    emit roundEnded(GameResult::Surrender, refund, handIndex, playerHands_.size());
}

void BlackjackGame::playerStand() {
    int nextHand = findNextPlayableHand(currentHandIndex_ + 1);
    if (nextHand != -1) {
        currentHandIndex_ = nextHand;
        emit playerTurn(currentHandIndex_, canDouble(), canSplit(), canSurrender());
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

    if (is21(playerHands_[currentHandIndex_])) {
        QTimer::singleShot(500, this, &BlackjackGame::playerStand);
    } else if (isLastCard) {
        // Stand after a short delay
        QTimer::singleShot(500, this, &BlackjackGame::playerStand);
    } else {
        // Normal split: emit playerTurn for the first split hand
        emit playerTurn(currentHandIndex_, canDouble(), canSplit(), canSurrender());
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

    hasRoundStarted_ = false;
}

Card BlackjackGame::drawCardFromShoe() {
    Card c = shoe_->draw();
    if (c.rank == Card::Rank::Cut) {
        needsShuffling_ = true;
        emit cutCardDrawn();
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

BasicStrategyChecker::PlayerAction BlackjackGame::getBestMove() const {
    BasicStrategyChecker::PlayerAction bestMove = strategyChecker_.getBestMove(playerHands_[currentHandIndex_], dealerHand_[0]);
    if (!canMakeAction(bestMove)) {
        BasicStrategyChecker::PlayerAction secondBestMove = strategyChecker_.getSecondBestMove(playerHands_[currentHandIndex_], dealerHand_[0]);
        if (!canMakeAction(secondBestMove))
            return strategyChecker_.getThirdBestMove(playerHands_[currentHandIndex_], dealerHand_[0]);
        return secondBestMove;
    }
    return bestMove;
}

bool BlackjackGame::canMakeAction(BasicStrategyChecker::PlayerAction action) const {
    switch (action) {
    case BasicStrategyChecker::PlayerAction::Split:
        return canSplit();
    case BasicStrategyChecker::PlayerAction::SplitIfDas:
        return rules_.doubleAfterSplit && canSplit();
    case BasicStrategyChecker::PlayerAction::Double:
        return canDouble();
    case BasicStrategyChecker::PlayerAction::Surrender:
        return canSurrender();
    default:
        return true;
    }
}

bool BlackjackGame::dealerHitsSoft17() const {
    return rules_.dealerHitsSoft17;
}

int BlackjackGame::findNextPlayableHand(int startIndex) const {
    for (int i = startIndex; i < playerHands_.size(); ++i) {
        const QVector<Card>& hand = playerHands_[i];  // ← Explicit const ref
        if (!BlackjackGame::isBust(const_cast<QVector<Card>&>(hand)) &&
            BlackjackGame::getHandValue(const_cast<QVector<Card>&>(hand)) < 21 &&
            !(BlackjackGame::getHandValue(const_cast<QVector<Card>&>(hand)) == 21 &&
              !rules_.hitSplitAces &&
              playerHands_[i][0].rank == Card::Rank::Ace)) {
            return i;
        }
    }
    return -1;  // No playable hands found
}


const QVector<Card>& BlackjackGame::getCurrentHand() const {
    return playerHands_[currentHandIndex_];
}

Card BlackjackGame::getDealerUpcard() const {
    return dealerHand_[0];
}

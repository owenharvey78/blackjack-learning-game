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

// Game state methods.

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

    if (isBust(playerHand)) {
        return GameResult::PlayerBust;
    }
    if (isBust(dealerHand)) {
        return GameResult::DealerBust;
    }
    if (isBlackJack(playerHand) && !isBlackJack(dealerHand)) {
        return GameResult::PlayerBlackjack;
    }
    if (isBlackJack(dealerHand) && !isBlackJack(playerHand)) {
        return GameResult::DealerWin;
    }

    if (playerValue > dealerValue) {
        return GameResult::PlayerWin;
    }
    else if (playerValue < dealerValue) {
        return GameResult::DealerWin;
    }
    else {
        return GameResult::Push;
    }
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

void BlackjackGame::dealerTurn(){
    while(dealerShouldHit(dealerHand_)){
        dealerHit();
        // Emit signal
    }
    dealerStand();
}

void BlackjackGame::dealerHit() {
    dealerHand_.push_back(drawCardFromShoe());
}

void BlackjackGame::dealerStand() {
    // Compare dealer hand to ALL player hands.
    for (int i = 0; i < playerHands_.size(); i++) {
        GameResult result = determineWinner(playerHands_[i], dealerHand_);
        checkCardsAndRound(result);
        // emit signal - must indicate WHICH hand won
    }}

void BlackjackGame::playerHit(){
    // Hit the active hand
    if (currentHandIndex_ >= playerHands_.size()) return; // safety check
    playerHands_[currentHandIndex_].append(drawCardFromShoe());

    // Emit signal
}

void BlackjackGame::playerStand(){
    // Check if the player has other hands
    if (currentHandIndex_ < playerHands_.size() - 1) {
        currentHandIndex_++;
        // Emit signal that active hand changed

    }
    else {
        dealerTurn();
    }
}

void BlackjackGame::playerSplit(){
    // ASSUMES PLAYER CAN SPLIT
    // BET WILL BE DOUBLED

    // Create new hand and move card to new hand.
    Card splitCard = playerHands_[currentHandIndex_].takeLast(); // Takes the second card from the hand
    QVector<Card> newHand;
    newHand.append(splitCard);
    playerHands_.insert(currentHandIndex_ + 1, newHand); // add the new hand to player hands

    // Deal a new card to both hands.
    playerHands_[currentHandIndex_].append(drawCardFromShoe());
    playerHands_[currentHandIndex_ + 1].append(drawCardFromShoe());

    // emit signal
}

void BlackjackGame::dealCards(){
    for(int i = 0; i < 2; i++){
        // Deal to player's hand.
        playerHands_[0].append(drawCardFromShoe());

        // Deal to dealer.
        dealerHand_.append(drawCardFromShoe());
    }
}

// Game Progression

void BlackjackGame::gameStart(){
    nextDeal();
}

void BlackjackGame::nextDeal(){
    // Check shuffling status.
    if(needsShuffling_){
        shoe_->shuffle();
        needsShuffling_ = false;
    }

    // reset necessary elements.
    playerHands_.clear();
    playerHands_.append(QVector<Card>());
    dealerHand_.clear();
    currentHandIndex_ = 0;

    dealCards();

    // Immediately check for dealer or player blackjack.
    if (isBlackJack(playerHands_[0]) || isBlackJack(dealerHand_)) {
        checkCardsAndRound(determineWinner(playerHands_[0], dealerHand_));
    }
}

void BlackjackGame::checkCardsAndRound(GameResult currentState){
    switch (currentState) {
    case GameResult::PlayerWin:
        //Emit signal
        break;

    case GameResult::DealerWin:
        //Emit signal
        break;

    case GameResult::PlayerBlackjack:
        //Emit signal
        break;

    case GameResult::PlayerBust:
        //Emit signal
        break;

    case GameResult::DealerBust:
        //Emit signal
        break;

    case GameResult::Push:
        //Emit signal
        break;
    }
}

Card BlackjackGame::drawCardFromShoe() {
    Card c = shoe_->draw();
    // Check if we hit the cut card
    if (c.rank == Card::Rank::Cut) {
        needsShuffling_ = true;
        c = shoe_->draw();
    }
    return c;
}

// TODO

#include "blackjack_game.h"
#include "shoe.h"
#include "card.h"

BlackJackGame::BlackJackGame(QObject *parent) : QObject{parent}{

    rules_ = Ruleset();
    shoe_ = new Shoe(rules_.numDecks, 0.75f, this);
    needsShuffling_ = true;
    hasRoundStarted_ = false;
    playerHands_ = QVector<QVector<Card>>();
    dealerHand_ = QVector<Card>();
    currentHandIndex_ = 0;

    // Add necessary connects here.
}

void BlackJackGame::setRuleset(Ruleset rules) {
    rules_ = rules;
}

// Game state methods.

int BlackJackGame::getHandValue(QVector<Card>& hand) {
    int value = 0;
    int aceCount = 0;

    for (const auto& card : hand) {
        value += card.getBlackjackValue();
        if (card.rank == Card::Rank::Ace) {
            aceCount++;
        }
    }

    while (value > 21 && aceCount > 0) {
        value -= 10; // Count ace as 1 instead of 11 to not bust.
        aceCount--;
    }

    return value;
}

bool BlackJackGame::isBust(QVector<Card>& hand) {
    return getHandValue(hand) > 21;
}

bool BlackJackGame::isBlackJack(const QVector<Card>& hand) {
    return hand.size() == 2 && getHandValue(const_cast<QVector<Card>&>(hand)) == 21;
}

bool BlackJackGame::is21(const QVector<Card> &hand) {
    return getHandValue(const_cast<QVector<Card>&>(hand)) == 21;
}

bool BlackJackGame::isSoftHand(const QVector<Card>& hand) {
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

bool BlackJackGame::canDouble(const QVector<Card>& hand, int currentSplitCount) const {
    return hand.size() == 2 && (rules_.doubleAfterSplit || currentSplitCount == 0);
}

bool BlackJackGame::canSurrender(const QVector<Card>& hand) const {
    return rules_.surrenderAllowed && hand.size() == 2;
}

bool BlackJackGame::canSplit(const QVector<Card>& hand, int currentSplitCount) const {
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

BlackJackGame::GameResult BlackJackGame::determineWinner(QVector<Card>& playerHand, QVector<Card>& dealerHand) {
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

bool BlackJackGame::dealerShouldHit(QVector<Card>& hand) const {
    int value = getHandValue(hand);
    if (value < 17) {
        return true;
    }
    if (value == 17 && rules_.dealerHitsSoft17 && isSoftHand(hand)) {
        return true;
    }
    return false;
}

void BlackJackGame::dealerTurn(){
    while(dealerShouldHit(dealerHand_)){
        dealerHit();
        // Emit Signal
    }

    if(rules_.dealerHitsSoft17){
        if(isSoftHand(dealerHand_) && getHandValue(dealerHand_) == 17){
            dealerHit();
            // Emit Signal
        }
    }
    dealerStand();
}

void BlackJackGame::dealerHit() {
    dealerHand_.push_back(shoe_->draw());
}

void BlackJackGame::dealerStand() {
    checkCardsAndRound(determineWinner(playerHands_[currentHandIndex_], dealerHand_));
}

void BlackJackGame::playerHit(){
    // Draw card for current hand
    Card c = shoe_->draw();
    if(c.rank == Card::Rank::Cut) {
        needsShuffling_ = true;
        c = shoe_->draw();
    }
    playerHands_[currentHandIndex_].append(c);

    // Emit signal
}

void BlackJackGame::playerStand(){
    // Check if the player has other hands
    if (currentHandIndex_ < playerHands_.size() - 1) {
        currentHandIndex_++;
        // Emit signal that active hand changed

    }
    else {
        dealerTurn();
    }
}

void BlackJackGame::playerSplit(){
    // ASSUMES PLAYER CAN SPLIT
    // BET WILL BE DOUBLED

    // add a new hand
    // - first card in hand stays
    // - second card in hand switched to new hand
    // deal a new card to the first hand THEN the second hand

    Card c = playerHands_[currentHandIndex_][1]; // gets the second card from hand
    playerHands_[currentHandIndex_].removeLast(); // remove that card from the list
    playerHands_.append(QVector<Card>()); // add a new hand
    playerHands_[currentHandIndex_ + 1].append(c); // move card to new hand



}

void BlackJackGame::dealCards(){
    // reset necessary elements
    playerHands_.clear();
    playerHands_.append(QVector<Card>());
    dealerHand_.clear();
    currentHandIndex_ = 0;

    for(int i = 0; i < 2; i++){
        // Deal to player first
        Card playerCard = shoe_->draw();
        if(playerCard.rank == Card::Rank::Cut) {
            needsShuffling_ = true;
            playerCard = shoe_->draw();
        }
        playerHands_[0].append(playerCard);

        // Deal to Dealer
        Card dealerCard = shoe_->draw();
        if(dealerCard.rank == Card::Rank::Cut) {
            needsShuffling_ = true;
            dealerCard = shoe_->draw();
        }
        dealerHand_.append(dealerCard);
    }
}

// Game Progression

void BlackJackGame::gameStart(){
    nextDeal();
}

void BlackJackGame::nextDeal(){

    if(needsShuffling_){
        shoe_->shuffle();
        needsShuffling_ = false;
    }

    playerHands_.clear();
    dealerHand_.clear();

    dealCards();
    for (int i = 0; i < playerHands_.length(); i++) {
        checkCardsAndRound(determineWinner(playerHands_[i], dealerHand_));
    }
}

void BlackJackGame::checkCardsAndRound(GameResult currentState){
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

Card BlackJackGame::drawCardFromShoe() {
    Card c = shoe_->draw();
    // Check if we hit the cut card
    if (c.rank == Card::Rank::Cut) {
        needsShuffling_ = true;
        c = shoe_->draw();
    }
    return c;
}

// TODO

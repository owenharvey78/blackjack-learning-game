#include "blackjack_game.h"
#include "shoe.h"
#include "card.h"

BlackJackGame::BlackJackGame(QObject *parent) : QObject{parent}{

    rules_ = Ruleset();
    shoe_ = new Shoe(rules_.numDecks, 0.75f, this);
    needsShuffling_ = true;
    playerHand_ = std::vector<Card>();
    dealerHand_ = std::vector<Card>();

    // Add necessary connects here.
}

void BlackJackGame::updateRules(Ruleset rules) {
    rules_ = rules;
}

// Game state methods.

int BlackJackGame::getHandValue(std::vector<Card>& hand) const {
    int value = 0;
    int aceCount = 0;

    for (const auto& card : hand) {
        value += card.getValue();
        if (card.isAce()) {
            aceCount++;
        }
    }

    while (value > 21 && aceCount > 0) {
        value -= 10; // Count ace as 1 instead of 11 to not bust. hehe.
        aceCount--;
    }

    return value;
}

bool BlackJackGame::isBust(std::vector<Card>& hand) const {
    return getHandValue(hand) > 21;
}

bool BlackJackGame::isBlackjack(const std::vector<Card>& hand)  const {
    return hand.size() == 2 && getHandValue(const_cast<std::vector<Card>&>(hand)) == 21;
}

bool BlackJackGame::is21(const std::vector<Card> &hand) const  {
    return getHandValue(const_cast<std::vector<Card>&>(hand)) == 21;
}

bool BlackJackGame::isSoftHand(const std::vector<Card>& hand) const {
    int value = 0;
    bool hasAce = false;

    for (const auto& card : hand) {
        value += card.getValue();
        if (card.isAce()) {
            hasAce = true;
        }
    }

    return hasAce && (value + 10) <= 21;
}

bool BlackJackGame::canDouble(const std::vector<Card>& hand) const {
    return hand.size() == 2;
}

bool BlackJackGame::canSurrender(const std::vector<Card>& hand) const {
    return rules_.surrenderAllowed && hand.size() == 2;
}

bool BlackJackGame::canSplit(const std::vector<Card>& hand, int currentSplitCount) const {
    if (hand.size() != 2) {
        return false;
    }
    if (hand[0].getRank() != hand[1].getRank()) {
        return false;
    }
    if (hand[0].isAce() && !rules_.resplitAces && currentSplitCount > 0) {
        return false;
    }
    return true;
}

BlackJackGame::GameResult BlackJackGame::determineWinner(std::vector<Card>& playerHand, std::vector<Card>& dealerHand) const {
    if (isBust(playerHand)) {
        return GameResult::PlayerBust;
    }
    if (isBust(dealerHand)) {
        return GameResult::DealerBust;
    }
    if (isBlackjack(playerHand) && !isBlackjack(dealerHand)) {
        return GameResult::PlayerBlackjack;
    }
    if (isBlackjack(dealerHand) && !isBlackjack(playerHand)) {
        return GameResult::DealerWin;
    }

    int playerValue = getHandValue(playerHand);
    int dealerValue = getHandValue(dealerHand);

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

bool BlackJackGame::dealerShouldHit(std::vector<Card>& hand) const {
    int value = getHandValue(hand);
    if (value < 17) {
        return true;
    }
    if (value == 17 && rules_.dealerHitsSoft17 && isSoftHand(hand)) {
        return true;
    }
    return false;
}

void BlackJackGame::dealerHit() {
    // TODO
}

void BlackJackGame::dealerStand() {
    // TODO
}

void BlackJackGame::dealCards(){
    for(int i = 0; i < 2; i++){
        Card drawnCard = shoe_->draw();

        // Player is dealt one card.
        // Dealer draws one card.
        // Then it's done again.
        if(drawnCard.getRank() == Card::Rank::Cut){
            playerHand_.push_back(shoe_->draw());
            needsShuffling_ = true;
        }
        else{
            playerHand_.push_back(drawnCard);
        }

        if(drawnCard.getRank() == Card::Rank::Cut){
            dealerHand_.push_back(shoe_->draw());
            needsShuffling_ = true;
        }
        else{
            dealerHand_.push_back(drawnCard);
        }
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

    playerHand_.clear();
    dealerHand_.clear();

    dealCards();
    checkCardsAndRound();
}

void BlackJackGame::checkCardsAndRound(){

}

// TODO

#include "blackjack_game.h"
#include "shoe.h"
#include "card.h"
#include <QTimer>

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

    // Account for aces
    while (value > 21 && aceCount > 0) {
        value -= 10; // Count ace as 1 instead of 11 to not bust.
        aceCount--; // Player still has ace but it is locked as a 1 now.
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

bool BlackJackGame::dealerShouldHit(QVector<Card>& hand) const {
    int value = getHandValue(hand);
    if (value < 17) {
        return true;
    }

    // Soft 17 rule
    if (value == 17 && rules_.dealerHitsSoft17 && isSoftHand(hand)) {
        return true;
    }
    return false;
}

void BlackJackGame::dealerTurn(){
    emit turnChanged(false, -1);

    // Start the recursive loop
    continueDealerTurn();
}

void BlackJackGame::continueDealerTurn() {
    if (dealerShouldHit(dealerHand_)) {
        dealerHit();
        // Recursively call this function after a delay
        QTimer::singleShot(1500, this, &BlackJackGame::continueDealerTurn);
    } else {
        // Stop hitting and resolve game
        dealerStand();
    }
}
void BlackJackGame::dealerHit() {
    Card c = drawCardFromShoe();
    dealerHand_.append(c);
    emit dealDealerCard(c);
}

void BlackJackGame::dealerStand() {
    // Compare dealer hand to ALL player hands.
    for (int i = 0; i < playerHands_.size(); i++) {
        GameResult result = determineWinner(playerHands_[i], dealerHand_);
        checkCardsAndRound(i, result);
    }
}

void BlackJackGame::playerHit(){
    // Hit the active hand
    if (currentHandIndex_ >= playerHands_.size()) return; // safety check
    Card c = drawCardFromShoe();
    playerHands_[currentHandIndex_].append(c);
    emit dealPlayerCard(currentHandIndex_, c);

    // Check if player busted immediately
    if (isBust(playerHands_[currentHandIndex_])) {
        playerStand();
    }
}

void BlackJackGame::playerDoubleDown() {
    if (currentHandIndex_ >= playerHands_.size()) return;
    if (!canDouble(playerHands_[currentHandIndex_], playerHands_.size() - 1)) return;

    // 1 Card only then forced stand.
    Card c = drawCardFromShoe();
    playerHands_[currentHandIndex_].append(c);
    emit dealPlayerCard(currentHandIndex_, c);

    // Forced stand after double
    playerStand();
}

void BlackJackGame::playerStand(){
    // Check if the player has other hands
    if (currentHandIndex_ < playerHands_.size() - 1) {
        currentHandIndex_++;
    }
    else {
        dealerTurn();
    }
}

void BlackJackGame::playerSplit(){
    // BET WILL BE DOUBLED

    // Ensure player can split
    if (!canSplit(playerHands_[currentHandIndex_], playerHands_.size() - 1)) return;

    // Create new hand and move card to new hand.
    Card splitCard = playerHands_[currentHandIndex_].takeLast(); // Takes the second card from the hand
    QVector<Card> newHand;
    newHand.append(splitCard);
    playerHands_.insert(currentHandIndex_ + 1, newHand); // add the new hand to player hands

    // Deal a new card to both hands.
    Card c1 = drawCardFromShoe();
    playerHands_[currentHandIndex_].append(c1);
    emit dealPlayerCard(currentHandIndex_, c1);

    Card c2 = drawCardFromShoe();
    playerHands_[currentHandIndex_ + 1].append(c2);
    emit dealPlayerCard(currentHandIndex_ + 1, c2);

    emit splitHand(playerHands_.size());

    // logic for split aces
    if (splitCard.rank == Card::Rank::Ace && !rules_.hitSplitAces) {
        // Force stand on current hand
        playerStand();

        // Automating the second ace stand:
        if (currentHandIndex_ < playerHands_.size() &&
            playerHands_[currentHandIndex_].first().rank == Card::Rank::Ace) {
            playerStand(); // Stand on the second ace too
        }
    }
}

// Game Progression

void BlackJackGame::startRound() {
    // Wrapper to ensure clean state
    dealNewHand();
}

void BlackJackGame::dealNewHand(){
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

    // Deal initial cards
    for(int i = 0; i < 2; i++){
        // Deal to player's hand.
        Card c = drawCardFromShoe();
        playerHands_[0].append(c);
        emit dealPlayerCard(0, c);

        // Deal to dealer.
        c = drawCardFromShoe();
        dealerHand_.append(c);
        emit dealDealerCard(c);
    }

    // Check Immediate Blackjack
    bool playerHasBJ = isBlackJack(playerHands_[0]);
    bool dealerHasBJ = isBlackJack(dealerHand_);

    if (playerHasBJ || dealerHasBJ) {
        // Game is over immediately
        checkCardsAndRound(0, determineWinner(playerHands_[0], dealerHand_));
    } else {
        emit turnChanged(true, 0); // Start player turn
    }
}

void BlackJackGame::checkCardsAndRound(int handIndex, GameResult currentState){
    emit roundOver(handIndex, currentState);
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

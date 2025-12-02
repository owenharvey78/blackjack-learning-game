#include "learn_widget.h"
#include "ui_learn_widget.h"

LearnWidget::LearnWidget(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::learn_widget)
{
    ui_->setupUi(this);

    headerList_ = {"Welcome to Blackjack!",
                   "Basic Goal",
                   "Card Values",
                   "Blackjack",
                   "Dealer Peek",
                   "Hitting and Standing",
                   "Double Down",
                   "Splitting Pairs",
                   "Surrender",
                   "End-of-Hand"
    };

    instructionList_ = {"",

                        "Your goal is simple, get as close to 21 without going over.\n "
                        "Beat the dealers cards to win!",

                        "Number cards are worth their number.\n"
                        "Face cards (J, Q, K) are worth 10.\n"
                        "Aces can be worth 1 or 10, depends on which helps your hand.",

                        "If your first two cards total 21, that is Blackjack!\n"
                        "Blackjack pays 3:2 (1.5x of what you bid)",

                        "If the dealers first card is an Ace, The dealer checks for Blackjack before you make a choice\n"
                        "This prevents you from running into a guarenteed loss",

                        "If you Hit, you get another card.\n"
                        "If you Stand, you keep the total in your hand.",

                        "After you get your first two cards you can Double Down.\n"
                        "You double your bet.\n"
                        "Then you receive exactly on card and stand with the total you have.",

                        "If your first two cards are the same you can Split into two hands.\n"
                        "Doubling after Splitting is allowed.\n"
                        "You cannot try and resplit aces.",

                        "You are able to Surrender, this means you will give up half your bet and end"
                        "the hand early.\n"
                        "This is only allowed before the dealer checks for BlackJack.",

                        "The round ends when you and the dealer stand or bust.\n"
                        "If you bust (go over 21), you lose.\n"
                        "If the dealer busts, you win.\n"
                        "The no one busts the total closer to 21 wins.\n"
                        "If there is a tie you keep your bet."
    };

    updatePage();

    connect(ui_->nextButton, &QPushButton::clicked, this, &LearnWidget::onNextButtonClicked);

    connect(ui_->previousButton, &QPushButton::clicked, this, &LearnWidget::onPrevButtonClicked);

    connect(ui_->mainMenuButton, &QPushButton::clicked, this, &LearnWidget::returnToMainMenu);
}

void LearnWidget::updatePage(){
    ui_->headerLabel->setText(headerList_[currentInstruction_]);
    ui_->instructionLabel->setText(instructionList_[currentInstruction_]);

    // diable previous button if we are on the first instruction
    ui_->previousButton->setDisabled(currentInstruction_ == 0);

    // diable next button if we are on the last page
    ui_->nextButton->setDisabled(currentInstruction_ == headerList_.size() - 1);
}

void LearnWidget::onNextButtonClicked(){
    if(currentInstruction_ != headerList_.size())
        currentInstruction_++;
    updatePage();
}

void LearnWidget::onPrevButtonClicked(){
    if(currentInstruction_ != 0)
        currentInstruction_--;
    updatePage();
}

LearnWidget::~LearnWidget()
{
    delete ui_;
}

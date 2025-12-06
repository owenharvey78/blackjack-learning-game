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
                   "End-of-Hand",
                   "Basic Strategies",
                   "Card Counting,"
                   "What's The Count?"
    };

    instructionList_ = {"",

                        "Your goal is simple, get as close to 21 without going over.\n "
                        "Beat the dealers cards to win!",

                        "Number cards are worth their number.\n"
                        "Face cards (J, Q, K) are worth 10.\n"
                        "Aces can be worth 1 or 10, depends on which helps your hand.",

                        "If your first two cards total 21, that is Blackjack!\n"
                        "Blackjack pays 3:2 (1.5x of what you bid)",

                        "If the dealers first card is an Ace, the dealer checks for\n Blackjack before you make a choice\n"
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
                        "If there is a tie you keep your bet.",

                        "If you would like to learn basic strategies press next.\n"
                        "Else head to the practice section to get some practice in!",

                        "Card counting helps you track whether a deck is rich in high cards (10's & Ace's)\n"
                        "or low cards. High cards are going to favor you.\n"
                        "\n"
                        "The counting system we implemented is called the Hi-Low Count: \n"
                        "\n"
                        "\t - Cards 2-6 -> +1\n"
                        "\t - Cards 7-9 ->  0\n"
                        "\t - 10, J, Q, K, A -> -1\n"
                        "\n"
                        "Running Count: \n \t Add the values of all visible cards.\n"
                        "True Count: \n \t The running count divided my the number of remaining decks.\n"
                        "\n"
                        "- A high true count tells us the deck is rich in 10's and Ace's.\n"
                        "- A negative true count tells us low cards mostly remain.\n"
                        "\n"
                        "It is strategic to increase your bet if the true count is positive.",

                        ""
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

    if(currentInstruction_ != 0){
        ui_->instructionLabel->setStyleSheet(QString("QLabel { color: white; font-family: 'Georgia'; "
                                                     "font-size: 18px;"
                                                     "background-color: #003b16;"
                                                     "border-radius: 20px;"
                                                     "border: 3px solid #ffd700;"
                                                     "}"));
    }else{
        ui_->instructionLabel->setStyleSheet(QString()); // This else feels redundant but helps. Trust
    }
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

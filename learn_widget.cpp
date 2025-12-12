#include "learn_widget.h"
#include "ui_learn_widget.h"

LearnWidget::LearnWidget(QWidget *parent)
    : QWidget(parent),
    ui_(new Ui::LearnWidget),
    currentInstruction_(0),
    cardSprites_(":/images/cards.png", 2.0),
    practiceDealerUpcard_(Card::Rank::Ace, Card::Suit::Clubs) {
    ui_->setupUi(this);

    scene_ = new QGraphicsScene(this);
    ui_->graphicsView->setScene(scene_);
    practiceGame_ = new BlackjackGame(this);


    /// Hide items
    ui_->spinBox->setVisible(false);
    ui_->checkButton->setVisible(false);
    ui_->practiceDealButton->setVisible(false);
    ui_->practiceHitButton->setVisible(false);
    ui_->practiceStandButton->setVisible(false);

    headerList_ = {
        "Welcome to Blackjack!",
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
        "Card Counting,",
        "What's The Count?",
        "Strategies:",
        "Practice Hit or Stand"
    };

    instructionList_ = {
        "",
        "Your goal is simple: get as close to 21 as possible without going over.\n "
        "Beat the dealer's cards to win!",

        "Number cards are worth their number.\n"
        "Face cards (J, Q, K) are worth 10.\n"
        "Aces can be worth 1 or 11—whichever helps your hand the most.",

        "If your first two cards total 21, that's a Blackjack!\n"
        "Usually, a Blackjack pays 3:2 ($3 for every $2 you bet).\n"
        "However, some casinos will offer something like a 6:5 payout instead.\n"
        "If the casino offers anything less than a 3:2 payout, DON'T PLAY AT THAT TABLE.\n"
        "These lowered payouts make it much more difficult for you to gain an advantage.",

        "If the dealer's first card is an Ace, the dealer checks for Blackjack before you make a choice.\n"
        "This prevents you from running into a guaranteed loss.",

        "If you Hit, another card will be dealt to your hand.\n"
        "If you Stand, you will keep the total in your hand.",

        "After you get your first two cards you can Double Down.\n"
        "You double your bet, then receive exactly one card and stand with the total you have.\n"
        "Some casinos allow you to double for less than your original bet, but if you're"
        " following basic strategy, you shouldn't do this.",

        "If your first two cards have the same value, you can Split into two hands.\n"
        "The rules for what you can do after splitting depend on the casino you're playing at.\n"
        "In this simulator, by default:\n"
        "\t- You can split any pair other than aces as many times as you want\n"
        "\t- Double after split (DAS) is allowed\n"
        "\t- After splitting aces, you only receive one card for each of the new hands",

        "Some casinos allow you to Surrender. This means you will give up half your bet and end"
        " the hand early.\n"
        "Most casinos only offer Late Surrender, meaning you can only surrender after the dealer"
        " checks for blackjack.",

        "The round ends when you and the dealer stand or bust.\n"
        "If you bust (go over 21), you lose.\n"
        "If the dealer busts, you win.\n"
        "If no one busts, whoevers hand total is closest to 21 wins.\n"
        "If your hand total is the same as the dealer's you keep your bet (this is called a \"push\").",

        "Head back to the menu to get some practice in!\n"
        "Once you're done, come back to learn about basic strategy and card counting.",

        "Card counting helps you track whether a deck is rich in high cards (10s and aces)\n"
        "or low cards. High cards are going to favor you.\n"
        "\n"
        "The counting system we implemented is called the Hi-Lo system. In this system: \n"
        "\n"
        "\t - Cards 2-6 count as +1\n"
        "\t - Cards 7-9 count as +0\n"
        "\t - 10, J, Q, K, A count as -1\n"
        "\n"
        "As you see cards being dealt, you'll add these values to a number in your head. That"
        " number is called the \"running count\".\n"
        "In contrast to the running count, the \"true count\" is the running count divided by"
        " the number of decks remaining.\n"
        "A high true count tells us the deck is rich in 10s and aces, while a negative true count"
        " tells us that the deck contains mostly low cards.\n"
        "\n"
        "Generally, you should bet the minimum amount if the true count is below one. If the true"
        " count is above 1, a common betting spread is to bet one \"betting unit\" for each positive"
        " true count. You can choose how much you want a betting unit to be based on the size of"
        " your bankroll. Larger betting units make you money at a faster rate, but increase the risk"
        " of ruin (ROR)—the chance of losing your entire bankroll. Smaller betting units are less"
        " profitable, but safer.",

        "If the current running count is a 0, and the current round shows an ace, a 2, and a 6,"
        " what will be the new running count?",

        "You can press the question mark icon in the practice section for a more detailed"
        " basic strategy chart. However, here are some basics to remember:\n"
        "- Always Stand on 17 or higher.\n"
        "- Always hit on anything 8 and lower.\n"
        "- Consider standing on 12 -16 if the dealer shows a weak upcard (2-6)\n"
        "since they are more likely to bust.\n"
        "- Consider on 12-16 if the dealer shows a strong upcard (7-Ace). \n\n"
        "- If insurance or even money is offered, never take it"
        "Doubling Down:\n"
        "- In H17 (dealer hits soft 17), always double down on 11 against any dealer's upcard.\n"
        "- Double on a 10 against a dealer's 2-9.\n"
        "- Always split aces and 8s.\n"
        "- Never split 5s or 10s. Two 5s make 10, which is better to double down, and"
        "two 10's give you a stong 20, which is a good hand to stand on.",

        "Here you will be dealt a hand.\n"
        "Decide wether you should hit or stand.\n\n"
        "This is not a full Blackjack game; just practice for you.\n\n"
        "Your cards are on the top row."
    };

    updatePage();

    connect(ui_->nextButton, &QPushButton::clicked, this, &LearnWidget::onNextButtonClicked);

    connect(ui_->previousButton, &QPushButton::clicked, this, &LearnWidget::onPrevButtonClicked);

    connect(ui_->mainMenuButton, &QPushButton::clicked, this, &LearnWidget::returnToMainMenu);

    connect(ui_->checkButton, &QPushButton::clicked, this, &LearnWidget::onCheckButtonClicked);

    connect(ui_->practiceDealButton, &QPushButton::clicked, this, &LearnWidget::startPracticeHand);

    connect(ui_->practiceHitButton, &QPushButton::clicked, this, &LearnWidget::onPracticeHitClicked);

    connect(ui_->practiceStandButton, &QPushButton::clicked, this, &LearnWidget::onPracticeStandClicked);
}

void LearnWidget::updatePage() {
    ui_->headerLabel->setText(headerList_[currentInstruction_]);
    ui_->instructionLabel->setText(instructionList_[currentInstruction_]);

    // diable previous button if we are on the first instruction
    ui_->previousButton->setDisabled(currentInstruction_ == 0);

    // diable next button if we are on the last page
    ui_->nextButton->setDisabled(currentInstruction_ == headerList_.size() - 1);

    // This if-else handles the style for the instruction label. 1st page looks wierd if we don't have this.
    if(currentInstruction_ != 0) {
        ui_->instructionLabel->setStyleSheet(QString("QLabel { color: white; font-family: 'Georgia'; "
                                                     "font-size: 18px;"
                                                     "background-color: #003b16;"
                                                     "border-radius: 20px;"
                                                     "border: 3px solid #ffd700;"
                                                     "}"));
    }
    else {
        ui_->instructionLabel->setStyleSheet(QString()); // This else feels redundant but helps. Trust
    }

    // Set up count page (pg 12)
    if(currentInstruction_ == 12) {
        ui_->spinBox->setVisible(true);
        ui_->checkButton->setVisible(true);
        showCountingExampleCards();
    }
    else {
        ui_->spinBox->setVisible(false);
        ui_->checkButton->setVisible(false);

        ui_->graphicsView->setVisible(false);
        scene_->clear();
    }

    // Set up practice on page 14
    if(currentInstruction_ == 14) {
        ui_->practiceDealButton->setVisible(true);
        ui_->practiceHitButton->setVisible(false);
        ui_->practiceStandButton->setVisible(false);
        ui_->graphicsView->setVisible(true);
        scene_->clear();
    }
    else {
        ui_->practiceDealButton->setVisible(false);
        ui_->practiceHitButton->setVisible(false);
        ui_->practiceStandButton->setVisible(false);
    }
}

void LearnWidget::startPracticeHand() {
    scene_->clear();
    practiceHand_.clear();

    // Draw two cards for the user
    practiceHand_.append(practiceGame_->drawCardFromShoe());
    practiceHand_.append(practiceGame_->drawCardFromShoe());

    // Draw card for dealer
    practiceDealerUpcard_ = practiceGame_->drawCardFromShoe();

    // Draws the cards onto the graphics view
    drawPracticeHand();

    ui_->instructionLabel->setText(QString("Decide: Hit or Stand"));

    ui_->practiceHitButton->setVisible(true);
    ui_->practiceStandButton->setVisible(true);
}

void LearnWidget::drawPracticeHand() {
    int x = 20;

    // Place cards onto the graphics view
    for(const Card& c : practiceHand_) {
        QPixmap face = cardSprites_.faceFor(c);
        QGraphicsPixmapItem* item = scene_->addPixmap(face);
        item->setPos(x, 20);
        x += 80;
    }

    // Place dealer card onto the graphics view
    QPixmap dealerFace = cardSprites_.faceFor(practiceDealerUpcard_);
    QGraphicsPixmapItem* item = scene_->addPixmap(dealerFace);
    item->setPos(20, 150);
}

void LearnWidget::onPracticeHitClicked() {
    practiceHand_.append(practiceGame_->drawCardFromShoe());

    // Redraw the cards
    drawPracticeHand();

    // Check if user busts
    if(practiceGame_->isHandBust(practiceHand_)) {
        ui_->instructionLabel->setText(QString("You busted! Try Again."));

        ui_->practiceHitButton->setVisible(false);
        ui_->practiceStandButton->setVisible(false);
    }
}

void::LearnWidget::onPracticeStandClicked() {
    int total = practiceGame_->playerHandValue(practiceHand_);
    ui_->instructionLabel->setText(QString("You stood on: ") + QString::number(total));

    ui_->practiceHitButton->setVisible(false);
    ui_->practiceStandButton->setVisible(false);
}

// Show counting cards is for the count example on pg 12
void LearnWidget::showCountingExampleCards() {
    scene_->clear();

    QVector<Card> exampleCards {
        Card(Card::Rank::Ace, Card::Suit::Spades),
        Card(Card::Rank::Two, Card::Suit::Hearts),
        Card(Card::Rank::Six, Card::Suit::Clubs)
    };

    const int width = 71;
    const int gap = 20;

    int totalWidth = exampleCards.size() * width + (exampleCards.size() - 1) * gap;
    int startX = (400 - totalWidth) / 2;

    // Draw cards onto the graphics view
    for(int i = 0; i < exampleCards.size(); i++) {
        QPixmap face = cardSprites_.faceFor(exampleCards[i]);
        QGraphicsPixmapItem* item = scene_->addPixmap(face);
        item->setPos(startX + i * (width + gap), 0);
    }

    ui_->graphicsView->setVisible(true);
}

void LearnWidget::onCheckButtonClicked() {
    if(ui_->spinBox->value() == 1) {
        ui_->instructionLabel->setText(QString("Correct! The Ace give us -1, 2 and 6 gives +1.\n So we end up with 1!"));
    }
    else {
        ui_->instructionLabel->setText(QString("Not quite since and Ace gives us -1, then 2 and 6 give +1.\n"
                                               "That then leaves us with 1.\n Good try tho"));
    }
}

void LearnWidget::onNextButtonClicked() {
    if(currentInstruction_ != headerList_.size())
        currentInstruction_++;
    updatePage();
}

void LearnWidget::onPrevButtonClicked() {
    if(currentInstruction_ != 0)
        currentInstruction_--;
    updatePage();
}

void LearnWidget::onMainMenuClicked() {
    emit returnToMainMenu();
}

LearnWidget::~LearnWidget() {
    delete ui_;
}

#include "learn_widget.h"
#include "ui_learn_widget.h"

LearnWidget::LearnWidget(QWidget *parent)
    : QWidget(parent),
    ui_(new Ui::LearnWidget),
    currentInstruction_(0),
    cardSprites_(":/images/cards.png", 2.0),
    practiceDealerUpcard_(Card::Rank::Ace, Card::Suit::Clubs)
{
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
        "Card Counting,",
        "What's The Count?",
        "Strategies:",
        "Practice Hit or Stand"
    };

    instructionList_ = {"",

        "Your goal is simple, get as close to 21 without going over.\n "
        "Beat the dealer's cards to win!",

        "Number cards are worth their number.\n"
        "Face cards (J, Q, K) are worth 10.\n"
        "Aces can be worth 1 or 11, depends on whichever helps your hand best.",

        "If your first two cards total 21, that is Blackjack!\n"
        "Blackjack pays 3:2 (1.5x of what you bid)",

        "If the dealer's first card is an Ace, the dealer checks for\n Blackjack before you make a choice\n"
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

        "What would be the running count for: Ace, 2, and 6?",

        "Basic Strategy:\n"
        "- Always Stand on 17 or higher.\n"
        "- Always hit on anything 11 and lower.\n"
        "- Consider standing on 12 -16 if the dealer shows a weak upcard (2-6)\n"
        "since they are more likely to bust.\n"
        "- Consider on 12-16 if the dealer shows a strong upcard (7-Ace). \n\n"
        "Doubling Down:\n"
        "- Always double down on 11 against any dealer's upcard.\n"
        "- Double Down on 10 against a dealer having a (2-9).\n"
        "- Always split Aces and 8's.\n"
        "- Never Split 5's or 10's, two 5's make 10, which is better to double down.\n"
        "Two 10's give you a stong 20.",

        "Here you will be dealt a hand.\n"
        "Decide wether you should hit or stand.\n\n"
        "This is not a full Blackjack game; just practice for you.\n\n"
        "Your cards on on the top row."
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

void LearnWidget::updatePage(){
    ui_->headerLabel->setText(headerList_[currentInstruction_]);
    ui_->instructionLabel->setText(instructionList_[currentInstruction_]);

    // diable previous button if we are on the first instruction
    ui_->previousButton->setDisabled(currentInstruction_ == 0);

    // diable next button if we are on the last page
    ui_->nextButton->setDisabled(currentInstruction_ == headerList_.size() - 1);

    // This if-else handles the style for the instruction label. 1st page looks wierd if we don't have this.
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

    // Set up count page (pg 12)
    if(currentInstruction_ == 12){
        ui_->spinBox->setVisible(true);
        ui_->checkButton->setVisible(true);
        showCountingExampleCards();
    }else{
        ui_->spinBox->setVisible(false);
        ui_->checkButton->setVisible(false);

        ui_->graphicsView->setVisible(false);
        scene_->clear();
    }

    // Set up practice on page 14
    if(currentInstruction_ == 14){
        ui_->practiceDealButton->setVisible(true);
        ui_->practiceHitButton->setVisible(false);
        ui_->practiceStandButton->setVisible(false);
        ui_->graphicsView->setVisible(true);
        scene_->clear();
    }else{
        ui_->practiceDealButton->setVisible(false);
        ui_->practiceHitButton->setVisible(false);
        ui_->practiceStandButton->setVisible(false);
    }
}

void LearnWidget::startPracticeHand(){
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

void LearnWidget::drawPracticeHand(){
    int x = 20;

    // Place cards onto the graphics view
    for(const Card& c : practiceHand_){
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

void LearnWidget::onPracticeHitClicked(){
    practiceHand_.append(practiceGame_->drawCardFromShoe());

    // Redraw the cards
    drawPracticeHand();

    // Check if user busts
    if(practiceGame_->isHandBust(practiceHand_)){
        ui_->instructionLabel->setText(QString("You busted! Try Again."));

        ui_->practiceHitButton->setVisible(false);
        ui_->practiceStandButton->setVisible(false);
    }
}

void::LearnWidget::onPracticeStandClicked(){
    int total = practiceGame_->playerHandValue(practiceHand_);
    ui_->instructionLabel->setText(QString("You stood on: ") + QString::number(total));

    ui_->practiceHitButton->setVisible(false);
    ui_->practiceStandButton->setVisible(false);
}

// Show counting cards is for the count example on pg 12
void LearnWidget::showCountingExampleCards(){
    scene_->clear();

    QVector<Card> exampleCards{
        Card(Card::Rank::Ace, Card::Suit::Spades),
        Card(Card::Rank::Two, Card::Suit::Hearts),
        Card(Card::Rank::Six, Card::Suit::Clubs)
    };

    const int width = 71;
    const int gap = 20;

    int totalWidth = exampleCards.size() * width + (exampleCards.size() - 1) * gap;
    int startX = (400 - totalWidth) / 2;

    // Draw cards onto the graphics view
    for(int i = 0; i < exampleCards.size(); i++){
        QPixmap face = cardSprites_.faceFor(exampleCards[i]);
        QGraphicsPixmapItem* item = scene_->addPixmap(face);
        item->setPos(startX + i * (width + gap), 0);
    }

    ui_->graphicsView->setVisible(true);
}

void LearnWidget::onCheckButtonClicked(){
    if(ui_->spinBox->value() == 1){
        ui_->instructionLabel->setText(QString("Correct! The Ace give us -1, 2 and 6 gives +1.\n So we end up with 1!"));
    }else{
        ui_->instructionLabel->setText(QString("Not quite since and Ace gives us -1, then 2 and 6 give +1.\n"
                                               "That then leaves us with 1.\n Good try tho"));
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

void LearnWidget::onMainMenuClicked(){
    emit returnToMainMenu();
}

LearnWidget::~LearnWidget()
{
    delete ui_;
}

#include "game_widget.h"
#include "ui_game_widget.h"
#include "strategy_chart_dialog.h"
#include <QVariantAnimation>

GameWidget::GameWidget(BlackjackGame* game, QWidget *parent)
    : QWidget(parent), ui_(new Ui::GameWidget), game_(game), balance_(1000),
    currentBetTotal_(0)
{
    ui_->setupUi(this);

    // Set up chip button icons (bottom row) - full size with no button styling
    ui_->chip1Button->setIcon(QIcon(QPixmap(":images/chip-1.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->chip1Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->chip1Button->setText("");
    ui_->chip1Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->chip5Button->setIcon(QIcon(QPixmap(":images/chip-5.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->chip5Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->chip5Button->setText("");
    ui_->chip5Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->chip10Button->setIcon(QIcon(QPixmap(":images/chip-10.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->chip10Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->chip10Button->setText("");
    ui_->chip10Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->chip25Button->setIcon(QIcon(QPixmap(":images/chip-25.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->chip25Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->chip25Button->setText("");
    ui_->chip25Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->chip50Button->setIcon(QIcon(QPixmap(":images/chip-50.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->chip50Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->chip50Button->setText("");
    ui_->chip50Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->chip100Button->setIcon(QIcon(QPixmap(":images/chip-100.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->chip100Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->chip100Button->setText("");
    ui_->chip100Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Set up bet display button icons (middle row) - full size with no button styling
    ui_->betDisplay1Button->setIcon(QIcon(QPixmap(":images/chip-1.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->betDisplay1Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->betDisplay1Button->setText("");
    ui_->betDisplay1Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->betDisplay5Button->setIcon(QIcon(QPixmap(":images/chip-5.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->betDisplay5Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->betDisplay5Button->setText("");
    ui_->betDisplay5Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->betDisplay10Button->setIcon(QIcon(QPixmap(":images/chip-10.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->betDisplay10Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->betDisplay10Button->setText("");
    ui_->betDisplay10Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->betDisplay25Button->setIcon(QIcon(QPixmap(":images/chip-25.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->betDisplay25Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->betDisplay25Button->setText("");
    ui_->betDisplay25Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->betDisplay50Button->setIcon(QIcon(QPixmap(":images/chip-50.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->betDisplay50Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->betDisplay50Button->setText("");
    ui_->betDisplay50Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->betDisplay100Button->setIcon(QIcon(QPixmap(":images/chip-100.png").scaled(CHIP_ICON_SIZE,
        CHIP_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->betDisplay100Button->setIconSize(QSize(CHIP_ICON_SIZE, CHIP_ICON_SIZE));
    ui_->betDisplay100Button->setText("");
    ui_->betDisplay100Button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Set up button icons for count, hint, and exit buttons (top right corner)

    // Show count button
    ui_->showCountButton->setIcon(QIcon(QPixmap(":images/count-button.png").scaled(
        EXIT_ICON_SIZE, EXIT_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->showCountButton->setIconSize(QSize(EXIT_ICON_SIZE, EXIT_ICON_SIZE));
    ui_->showCountButton->setText("");
    ui_->showCountButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Strategy chart button
    ui_->strategyChartButton->setIcon(QIcon(QPixmap(":images/hint-button.png").scaled(
        EXIT_ICON_SIZE, EXIT_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->strategyChartButton->setIconSize(QSize(EXIT_ICON_SIZE, EXIT_ICON_SIZE));
    ui_->strategyChartButton->setText("");
    ui_->strategyChartButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Return to menu button
    ui_->returnButton->setIcon(QIcon(QPixmap(":images/exit-button.png").scaled(
        EXIT_ICON_SIZE, EXIT_ICON_SIZE, Qt::KeepAspectRatio)));
    ui_->returnButton->setIconSize(QSize(EXIT_ICON_SIZE, EXIT_ICON_SIZE));
    ui_->returnButton->setText("");
    ui_->returnButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Set up gameplay action button icons (hit, stand, double, split)

    // Hit button
    ui_->hitButton->setIcon(QIcon(QPixmap(":images/hit-button.png").scaled(
        GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE, Qt::KeepAspectRatio)));
    ui_->hitButton->setIconSize(QSize(GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE));
    ui_->hitButton->setText("");
    ui_->hitButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Stand button
    ui_->standButton->setIcon(QIcon(QPixmap(":images/stand-button.png").scaled(
        GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE, Qt::KeepAspectRatio)));
    ui_->standButton->setIconSize(QSize(GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE));
    ui_->standButton->setText("");
    ui_->standButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Double button
    ui_->doubleButton->setIcon(QIcon(QPixmap(":images/double-button.png").scaled(
        GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE, Qt::KeepAspectRatio)));
    ui_->doubleButton->setIconSize(QSize(GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE));
    ui_->doubleButton->setText("");
    ui_->doubleButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Split button
    ui_->splitButton->setIcon(QIcon(QPixmap(":images/split-button.png").scaled(
        GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE, Qt::KeepAspectRatio)));
    ui_->splitButton->setIconSize(QSize(GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE));
    ui_->splitButton->setText("");
    ui_->splitButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Surrender button
    ui_->surrenderButton->setIcon(QIcon(QPixmap(":images/surrender-button.png").scaled(
        GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE, Qt::KeepAspectRatio)));
    ui_->surrenderButton->setIconSize(QSize(GAMEPLAY_BUTTON_SIZE, GAMEPLAY_BUTTON_SIZE));
    ui_->surrenderButton->setText("");
    ui_->surrenderButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    ui_->hitButton->hide();
    ui_->standButton->hide();
    ui_->doubleButton->hide();
    ui_->splitButton->hide();
    ui_->surrenderButton->hide();

    // Set up count label (programmatically, to avoid interfering with rest of layout)
    countLabel_ = new QLabel(this);
    countLabel_->setStyleSheet("font-size: 24pt; font-weight: bold; color: white; background-color: transparent;");
    countLabel_->hide();

    // Set up result label (programmatically, to avoid interfering with rest of layout)
    resultLabel_ = new QLabel(this);
    resultLabel_->setStyleSheet("font-size: 36pt; font-weight: bold; color: white; background-color: transparent;");
    resultLabel_->setAlignment(Qt::AlignCenter);
    resultLabel_->hide();

    // Set up start round button
    startRoundOnIconOne_ = true;
    startRoundIconOne_ = QIcon(QPixmap(":images/begin-round-1.png").scaled(
        4 * START_BUTTON_HEIGHT, START_BUTTON_HEIGHT, Qt::KeepAspectRatio));
    startRoundIconTwo_ = QIcon(QPixmap(":images/begin-round-2.png").scaled(
        4 * START_BUTTON_HEIGHT, START_BUTTON_HEIGHT, Qt::KeepAspectRatio));
    ui_->startRoundButton->setIconSize(QSize(4 * START_BUTTON_HEIGHT, START_BUTTON_HEIGHT));
    ui_->startRoundButton->setIcon(startRoundIconOne_);
    ui_->startRoundButton->setText("");
    ui_->startRoundButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    flashAnimationTimer_ = new QTimer(this);
    connect(flashAnimationTimer_, &QTimer::timeout, this, [this]() {
        ui_->startRoundButton->setIcon(startRoundOnIconOne_ ? startRoundIconOne_ : startRoundIconTwo_);
        startRoundOnIconOne_ = !startRoundOnIconOne_;
    });
    flashAnimationTimer_->setInterval(START_BUTTON_FLASH_DURATION);
    flashAnimationTimer_->start();

    // Set up CardsView
    cardsView_ = new CardsView(this);
    cardsView_->setGeometry(rect());
    cardsView_->lower();

    // Set up current bet map
    currentBet_[1] = 0;
    currentBet_[5] = 0;
    currentBet_[10] = 0;
    currentBet_[25] = 0;
    currentBet_[50] = 0;
    currentBet_[100] = 0;

    // Initialize strategy chart overlay
    strategyOverlay_ = new StrategyChartDialog(game_->dealerHitsSoft17(), this);

    // Strategy chart button
    connect(ui_->strategyChartButton, &QPushButton::clicked, this, &GameWidget::onStrategyChartButtonClicked);

    // Set up connections for chip buttons
    connect(ui_->chip1Button, &QPushButton::clicked, this, [this]() { addChip(1); });
    connect(ui_->chip5Button, &QPushButton::clicked, this, [this]() { addChip(5); });
    connect(ui_->chip10Button, &QPushButton::clicked, this, [this]() { addChip(10); });
    connect(ui_->chip25Button, &QPushButton::clicked, this, [this]() { addChip(25); });
    connect(ui_->chip50Button, &QPushButton::clicked, this, [this]() { addChip(50); });
    connect(ui_->chip100Button, &QPushButton::clicked, this, [this]() { addChip(100); });

    connect(ui_->betDisplay1Button, &QPushButton::clicked, this, [this]() { removeChip(1); });
    connect(ui_->betDisplay5Button, &QPushButton::clicked, this, [this]() { removeChip(5); });
    connect(ui_->betDisplay10Button, &QPushButton::clicked, this, [this]() { removeChip(10); });
    connect(ui_->betDisplay25Button, &QPushButton::clicked, this, [this]() { removeChip(25); });
    connect(ui_->betDisplay50Button, &QPushButton::clicked, this, [this]() { removeChip(50); });
    connect(ui_->betDisplay100Button, &QPushButton::clicked, this, [this]() { removeChip(100); });

    // Starting/ending game.
    connect(ui_->startRoundButton, &QPushButton::clicked, this, &GameWidget::onStartButtonClicked);
    connect(this, &GameWidget::beginRound, game_, &BlackjackGame::beginRound);
    connect(game_, &BlackjackGame::roundEnded, this, &GameWidget::onRoundEnded);
    connect(game_, &BlackjackGame::playerTurn, this, &GameWidget::onPlayerTurn);
    connect(game_, &BlackjackGame::dealerTurnStarted, this, &GameWidget::onDealerTurnStarted);
    connect(game_, &BlackjackGame::betPlaced, this, &GameWidget::onBetPlaced);
    connect(game_, &BlackjackGame::splitHand, this, &GameWidget::onHandSplit);

    // Card deals.
    connect(game, &BlackjackGame::playerCardDealt, this, &GameWidget::onPlayerCardDealt);
    connect(game, &BlackjackGame::dealerCardDealt, this, &GameWidget::onDealerCardDealt);
    connect(game, &BlackjackGame::cutCardDrawn, cardsView_, &CardsView::drawCutCard);

    // Show Count
    connect(ui_->showCountButton, &QPushButton::clicked, this, &GameWidget::toggleCountingLabel);

    // Button presses.
    connect(ui_->hitButton, &QPushButton::clicked, this, [this]{
        checkBasicStrategy(BasicStrategyChecker::PlayerAction::Hit);
        game_->playerHit();
        ui_->surrenderButton->setEnabled(false);
    });
    connect(ui_->standButton, &QPushButton::clicked, this, [this]{
        checkBasicStrategy(BasicStrategyChecker::PlayerAction::Stand);
        game_->playerStand();
    });
    connect(ui_->doubleButton, &QPushButton::clicked, this, [this]{
        checkBasicStrategy(BasicStrategyChecker::PlayerAction::Double);
        game_->playerDouble();
    });
    connect(ui_->splitButton, &QPushButton::clicked, this, [this]{
        checkBasicStrategy(BasicStrategyChecker::PlayerAction::Split);
        game_->playerSplit();
    });
    connect(ui_->returnButton, &QPushButton::clicked, this, &GameWidget::onReturnToMainMenu);
    connect(ui_->surrenderButton, &QPushButton::clicked, this, [this]{
        checkBasicStrategy(BasicStrategyChecker::PlayerAction::Surrender);
        game_->playerSurrender();
    });

    // Tells QT to use the background color from the stylesheet
    setAttribute(Qt::WA_StyledBackground, true);
}

GameWidget::~GameWidget() {
    delete ui_;
}

QString GameWidget::actionToString(BasicStrategyChecker::PlayerAction action) const {
    switch (action) {
    case BasicStrategyChecker::PlayerAction::Hit:
        return "Hit";
    case BasicStrategyChecker::PlayerAction::Stand:
        return "Stand";
    case BasicStrategyChecker::PlayerAction::Double:
        return "Double Down";
    case BasicStrategyChecker::PlayerAction::Split:
        return "Split";
    case BasicStrategyChecker::PlayerAction::SplitIfDas:
        return "Split (if DAS allowed)";
    case BasicStrategyChecker::PlayerAction::Surrender:
        return "Surrender";
    }
    return "Unknown";
}

QString GameWidget::generateStrategyExplanation(BasicStrategyChecker::PlayerAction recommendedAction,
                                                const QVector<Card>& hand, int dealerUpcard) const {
    QString action = actionToString(recommendedAction).toLower();
    QString handDescription;
    QString dealerCard;

    // Convert dealer upcard to readable string
    if (dealerUpcard == 11) dealerCard = "Ace";
    else if (dealerUpcard == 10) dealerCard = "10";
    else dealerCard = QString::number(dealerUpcard);

    // Determine hand description
    if (hand.size() == 2 && hand[0].getBlackjackValue() == hand[1].getBlackjackValue()) {
        // It's a pair
        QString rankName;
        if (hand[0].rank == Card::Rank::Ace) rankName = "Aces";
        else if (hand[0].rank == Card::Rank::Two) rankName = "2s";
        else if (hand[0].rank == Card::Rank::Three) rankName = "3s";
        else if (hand[0].rank == Card::Rank::Four) rankName = "4s";
        else if (hand[0].rank == Card::Rank::Five) rankName = "5s";
        else if (hand[0].rank == Card::Rank::Six) rankName = "6s";
        else if (hand[0].rank == Card::Rank::Seven) rankName = "7s";
        else if (hand[0].rank == Card::Rank::Eight) rankName = "8s";
        else if (hand[0].rank == Card::Rank::Nine) rankName = "9s";
        else rankName = "10s"; // 10, J, Q, K all have value 10

        handDescription = QString("a pair of %1").arg(rankName);
    } else {
        // Not a pair - check if soft or hard
        int handValue = BlackjackGame::getHandValue(const_cast<QVector<Card>&>(hand));
        bool isSoft = BlackjackGame::isSoftHand(hand);
        QString handType = isSoft ? "soft" : "hard";
        handDescription = QString("%1 %2").arg(handType).arg(handValue);
    }

    return QString("basic strategy recommends %1ing with %2 against a dealer %3")
        .arg(action, handDescription, dealerCard);
}

bool GameWidget::checkBasicStrategy(BasicStrategyChecker::PlayerAction chosenAction) {
    // Get the recommended move from basic strategy
    BasicStrategyChecker::PlayerAction recommendedAction = game_->getBestMove();

    // If player's choice matches recommendation, proceed without warning
    if (chosenAction == recommendedAction) {
        return true;
    }

    // Get hand information for detailed explanation
    const QVector<Card>& currentHand = game_->getCurrentHand();
    int dealerUpcard = game_->getDealerUpcard().getBlackjackValue();

    // Generate detailed explanation
    QString chosenStr = actionToString(chosenAction);
    QString explanation = generateStrategyExplanation(recommendedAction, currentHand, dealerUpcard);

    QMessageBox messageBox(this);
    messageBox.setWindowTitle("Basic Strategy Mistake");
    messageBox.setText(QString("You chose to %1, but %2.")
                   .arg(chosenStr.toLower(), explanation));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.exec();

    // Always return true - we still execute their chosen action
    return true;
}

void GameWidget::onRoundEnded(BlackjackGame::GameResult result, int payout,
                               int handIndex, int totalHands) {
    QString message;

    switch (result) {
    case BlackjackGame::GameResult::Win:
        message = "Win";
        break;
    case BlackjackGame::GameResult::Lose:
        message = "Lose";
        break;
    case BlackjackGame::GameResult::Push:
        message = "Push";
        break;
    case BlackjackGame::GameResult::Blackjack:
        message = "Blackjack!";
        break;
    case BlackjackGame::GameResult::Surrender:
        message = "Surrender";
        break;
    }

    // Add hand number if multiple hands
    if (totalHands > 1) {
        message = QString("Hand %1: %2").arg(handIndex + 1).arg(message);
    }

    // Display result message
    resultLabel_->setText(message);
    resultLabel_->adjustSize();
    resultLabel_->move((width() - resultLabel_->width()) / 2, (height() - resultLabel_->height()) / 2);
    resultLabel_->show();

    updateBalance(payout, 1000, 500);

    if (balance_ == 0) {
        timer_.singleShot(2000, this, [this]() {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("YOU HAVE NO MONEY!");
            msgBox.setText("Do you want to restart?");

            QPushButton* restartButton =
                msgBox.addButton("Yes, restart", QMessageBox::AcceptRole);
            QPushButton* menuButton =
                msgBox.addButton("No, return to menu", QMessageBox::RejectRole);

            msgBox.exec();

            if (msgBox.clickedButton() == restartButton) {
                balance_ = 1000;
                ui_->balanceLabel->setText(QString("$%1").arg(balance_));
                game_->setShuffling(true);
                GameWidget::resetGame();
            }
            else if (msgBox.clickedButton() == menuButton) {
                emit returnToMainMenu();
            }
        });
    }

    // Only reset game after the LAST hand is processed
    if (handIndex == totalHands - 1) {
        // Add extra delay after last hand before reset
        timer_.singleShot(2000, this, &GameWidget::resetGame);
    }

    // Remove split hand selection.
    cardsView_->setHasSplit(false);
}

void GameWidget::beginBetStage() {
    // Show betting buttons
    ui_->startRoundButton->show();
    ui_->startRoundButton->setEnabled(false);
    flashAnimationTimer_->stop();
    ui_->chip1Button->show();
    ui_->chip5Button->show();
    ui_->chip10Button->show();
    ui_->chip25Button->show();
    ui_->chip50Button->show();
    ui_->chip100Button->show();
    setChipButtonsEnabled();

    // Hide bet text (without shifting layout)
    ui_->balanceUpdateLabel->show();
    ui_->balanceUpdateLabel->setText("");

    // Hide gameplay buttons
    ui_->hitButton->hide();
    ui_->standButton->hide();
    ui_->doubleButton->hide();
    ui_->splitButton->hide();
    ui_->surrenderButton->hide();
}

void GameWidget::addChip(int value) {
    // Update current bet
    currentBetTotal_ += value;
    currentBet_[value]++;

    // Enable "Start Round" button
    ui_->startRoundButton->setEnabled(true);
    flashAnimationTimer_->start();

    // Add button to view so players can remove chip from bet and show count
    switch (value) {
    case 1:
        ui_->betDisplay1Button->show();
        ui_->betDisplay1CountLabel->show();
        ui_->betDisplay1CountLabel->setText(QString("x%1").arg(currentBet_[value]));
        break;
    case 5:
        ui_->betDisplay5Button->show();
        ui_->betDisplay5CountLabel->show();
        ui_->betDisplay5CountLabel->setText(QString("x%1").arg(currentBet_[value]));
        break;
    case 10:
        ui_->betDisplay10Button->show();
        ui_->betDisplay10CountLabel->show();
        ui_->betDisplay10CountLabel->setText(QString("x%1").arg(currentBet_[value]));
        break;
    case 25:
        ui_->betDisplay25Button->show();
        ui_->betDisplay25CountLabel->show();
        ui_->betDisplay25CountLabel->setText(QString("x%1").arg(currentBet_[value]));
        break;
    case 50:
        ui_->betDisplay50Button->show();
        ui_->betDisplay50CountLabel->show();
        ui_->betDisplay50CountLabel->setText(QString("x%1").arg(currentBet_[value]));
        break;
    case 100:
        ui_->betDisplay100Button->show();
        ui_->betDisplay100CountLabel->show();
        ui_->betDisplay100CountLabel->setText(QString("x%1").arg(currentBet_[value]));
        break;
    default:
        break;  // Should never run
    }

    // Disable any necessary buttons
    setChipButtonsEnabled();
}

void GameWidget::removeChip(int value) {
    // Update current bet
    currentBetTotal_ -= value;
    int newChipCount = --currentBet_[value];

    // Update visibility and count on remove button
    switch (value) {
    case 1:
        if (newChipCount == 0) {
            ui_->betDisplay1Button->hide();
            ui_->betDisplay1CountLabel->hide();
        } else {
            ui_->betDisplay1CountLabel->setText(QString("x%1").arg(newChipCount));
        }
        break;
    case 5:
        if (newChipCount == 0) {
            ui_->betDisplay5Button->hide();
            ui_->betDisplay5CountLabel->hide();
        } else {
            ui_->betDisplay5CountLabel->setText(QString("x%1").arg(newChipCount));
        }
        break;
    case 10:
        if (newChipCount == 0) {
            ui_->betDisplay10Button->hide();
            ui_->betDisplay10CountLabel->hide();
        } else {
            ui_->betDisplay10CountLabel->setText(QString("x%1").arg(newChipCount));
        }
        break;
    case 25:
        if (newChipCount == 0) {
            ui_->betDisplay25Button->hide();
            ui_->betDisplay25CountLabel->hide();
        } else {
            ui_->betDisplay25CountLabel->setText(QString("x%1").arg(newChipCount));
        }
        break;
    case 50:
        if (newChipCount == 0) {
            ui_->betDisplay50Button->hide();
            ui_->betDisplay50CountLabel->hide();
        } else {
            ui_->betDisplay50CountLabel->setText(QString("x%1").arg(newChipCount));
        }
        break;
    case 100:
        if (newChipCount == 0) {
            ui_->betDisplay100Button->hide();
            ui_->betDisplay100CountLabel->hide();
        } else {
            ui_->betDisplay100CountLabel->setText(QString("x%1").arg(newChipCount));
        }
        break;
    default:
        break;  // Should never run
    }

    // Update current bet label and disable start button if necessary
    if (currentBetTotal_ == 0) {
        ui_->startRoundButton->setEnabled(false);
        flashAnimationTimer_->stop();
    }

    // Enable any necessary chip buttons
    setChipButtonsEnabled();
}

void GameWidget::setChipButtonsEnabled() {
    int newBalance = balance_ - currentBetTotal_;
    ui_->chip1Button->setEnabled(newBalance >= 1);
    ui_->chip5Button->setEnabled(newBalance >= 5);
    ui_->chip10Button->setEnabled(newBalance >= 10);
    ui_->chip25Button->setEnabled(newBalance >= 25);
    ui_->chip50Button->setEnabled(newBalance >= 50);
    ui_->chip100Button->setEnabled(newBalance >= 100);
}

void GameWidget::onStartButtonClicked() {
    // Hide start round button
    ui_->startRoundButton->hide();

    // Gameplay buttons will be shown by onTurnChanged signal

    // remove betting buttons and labels
    ui_->chip1Button->hide();
    ui_->chip5Button->hide();
    ui_->chip10Button->hide();
    ui_->chip25Button->hide();
    ui_->chip50Button->hide();
    ui_->chip100Button->hide();
    ui_->betDisplay1Button->hide();
    ui_->betDisplay1CountLabel->hide();
    ui_->betDisplay5Button->hide();
    ui_->betDisplay5CountLabel->hide();
    ui_->betDisplay10Button->hide();
    ui_->betDisplay10CountLabel->hide();
    ui_->betDisplay25Button->hide();
    ui_->betDisplay25CountLabel->hide();
    ui_->betDisplay50Button->hide();
    ui_->betDisplay50CountLabel->hide();
    ui_->betDisplay100Button->hide();
    ui_->betDisplay100CountLabel->hide();
    emit beginRound(currentBetTotal_);

    // Update balance label with current balance after bet
    currentBet_[1] = 0;
    currentBet_[5] = 0;
    currentBet_[10] = 0;
    currentBet_[25] = 0;
    currentBet_[50] = 0;
    currentBet_[100] = 0;   // TODO: refactor this into a helper method
    currentBetTotal_ = 0;
}

void GameWidget::onPlayerCardDealt(Card card, int handIndex, bool isLastCard) {
    cardsView_->dealPlayerCard(card, handIndex, isLastCard);
    updateCountingLabel();
}

void GameWidget::onDealerCardDealt(Card card) {
    cardsView_->dealDealerCard(card);
    updateCountingLabel();
}

void GameWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    // Resize cardsView
    cardsView_->setGeometry(rect());

    // Resize overlay if visible
    if (strategyOverlay_ && strategyOverlay_->isVisible()) {
        strategyOverlay_->setGeometry(rect());
    }

    // Reposition result label if visible
    if (resultLabel_->isVisible()) {
        resultLabel_->move((width() - resultLabel_->width()) / 2, (height() - resultLabel_->height()) / 2);
    }

    // Resize balance label if necessary
    updateCountingLabel();
}

void GameWidget::resetGame() {
    // Reset cardsView
    cardsView_->cleanUp();

    // Reset variables.
    currentBetTotal_ = 0;

    // Reset chips.
    QMapIterator<int, int> i(currentBet_);
    while (i.hasNext()) {
        i.next();
        currentBet_[i.key()] = 0;
    }

    // Hide chip buttons and count labels.
    ui_->betDisplay1Button->hide();
    ui_->betDisplay1CountLabel->hide();
    ui_->betDisplay5Button->hide();
    ui_->betDisplay5CountLabel->hide();
    ui_->betDisplay10Button->hide();
    ui_->betDisplay10CountLabel->hide();
    ui_->betDisplay25Button->hide();
    ui_->betDisplay25CountLabel->hide();
    ui_->betDisplay50Button->hide();
    ui_->betDisplay50CountLabel->hide();
    ui_->betDisplay100Button->hide();
    ui_->betDisplay100CountLabel->hide();

    // Hide result label
    resultLabel_->hide();

    beginBetStage();
}

void GameWidget::onPlayerTurn(int handIndex, bool canDouble, bool canSplit, bool canSurrender) {
    // Track which hand is active to support multi-hand logic
    currentHandIndex_ = handIndex;
    cardsView_->setCurrentHandIndex(handIndex);

    // Show gameplay buttons for player's turn
    ui_->hitButton->show();
    ui_->standButton->show();
    ui_->showCountButton->show();

    // Show double/split buttons based on game logic decision
    ui_->doubleButton->setVisible(canDouble);
    ui_->splitButton->setVisible(canSplit);
    ui_->surrenderButton->setVisible(canSurrender);
}

void GameWidget::onDealerTurnStarted() {
    // Hide all gameplay buttons during dealer's turn
    ui_->hitButton->hide();
    ui_->standButton->hide();
    ui_->doubleButton->hide();
    ui_->splitButton->hide();
    ui_->surrenderButton->hide();

    // Hide chip buttons and labels
    ui_->betDisplay1Button->hide();
    ui_->betDisplay1CountLabel->hide();
    ui_->betDisplay5Button->hide();
    ui_->betDisplay5CountLabel->hide();
    ui_->betDisplay10Button->hide();
    ui_->betDisplay10CountLabel->hide();
    ui_->betDisplay25Button->hide();
    ui_->betDisplay25CountLabel->hide();
    ui_->betDisplay50Button->hide();
    ui_->betDisplay50CountLabel->hide();
    ui_->betDisplay100Button->hide();
    ui_->betDisplay100CountLabel->hide();

    // Flip hole card
    cardsView_->flipDealerHoleCard();
}

void GameWidget::onHandSplit(int handIndex) {
    cardsView_->handleHandSplit(handIndex);
}

void GameWidget::onBetPlaced(int betAmount) {
    updateBalance(-betAmount, 1000, 500);
}

void GameWidget::toggleCountingLabel() {
    showingCountLabel_ = !showingCountLabel_;
    countLabel_->setVisible(showingCountLabel_);
    updateCountingLabel();
}

void GameWidget::onReturnToMainMenu() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exiting main menu?", "Are you sure you want to return to the main menu?",
                                                                QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit returnToMainMenu();
    }
    else if (reply == QMessageBox::No) {
        // Do nothing pretty much.
    }
}

void GameWidget::onStrategyChartButtonClicked() {
    strategyOverlay_->showOverlay();
}

void GameWidget::updateCountingLabel() {
    if (countLabel_->isVisible()) {
        QString runningCount = QString::number(game_->getRunningCount());
        QString trueCount = QString::number(std::round(game_->getTrueCount() * 100)/100); // Round to 2 decimal places

        QString labelText = QString("Running Count: %1\nTrue Count: %2")
                                .arg(runningCount, trueCount);

        countLabel_->setText(labelText);
        countLabel_->adjustSize();  // Force Qt to calculate the label size based on text
        countLabel_->move(width() - countLabel_->width() - 10, height() / 2 - countLabel_->height());
    }
}

void GameWidget::updateBalance(int updateAmount, int updateDelay, int animationDuration) {
    if (updateAmount == 0) return;
    int originalBalance = balance_;
    balance_ += updateAmount;

    if (updateAmount < 0) {
        ui_->balanceUpdateLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #ff4444;");
        ui_->balanceUpdateLabel->setText(QString("-$%1").arg(-updateAmount));
    }
    else {
        ui_->balanceUpdateLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #008f15;");
        ui_->balanceUpdateLabel->setText(QString("+$%1").arg(updateAmount));
    }

    QVariantAnimation* labelUpdateAnimation = new QVariantAnimation(this);
    labelUpdateAnimation->setDuration(animationDuration);
    labelUpdateAnimation->setStartValue(0);
    labelUpdateAnimation->setEndValue(updateAmount);

    connect(labelUpdateAnimation, &QVariantAnimation::valueChanged, this,
            [this, originalBalance] (const QVariant& v) {
        const int amountToUpdate = v.toInt();
        if (amountToUpdate < 0) {
            ui_->balanceUpdateLabel->setText(QString("-$%1").arg(-amountToUpdate));
        }
        else {
            ui_->balanceUpdateLabel->setText(QString("+$%1").arg(amountToUpdate));
        }
        ui_->balanceLabel->setText(QString("$%1").arg(originalBalance + amountToUpdate));
    });
    connect(labelUpdateAnimation, &QVariantAnimation::finished, this,
            [this] () {
        ui_->balanceUpdateLabel->setText("");
        ui_->balanceLabel->setText(QString("$%1").arg(balance_));
    });

    QTimer::singleShot(updateDelay, [labelUpdateAnimation] () { labelUpdateAnimation->start(QAbstractAnimation::DeleteWhenStopped); });
}

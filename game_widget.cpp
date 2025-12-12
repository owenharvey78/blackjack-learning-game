#include "game_widget.h"
#include "ui_game_widget.h"
#include "strategy_chart_dialog.h"

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

    ui_->hitButton->setVisible(false);
    ui_->standButton->setVisible(false);
    ui_->showCountLabel->setVisible(false);
    ui_->showCountButton->setVisible(false);

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
    connect(ui_->betAllButton, &QPushButton::clicked, this, &GameWidget::onAllIn);

    connect(ui_->betDisplay1Button, &QPushButton::clicked, this, [this]() { removeChip(1); });
    connect(ui_->betDisplay5Button, &QPushButton::clicked, this, [this]() { removeChip(5); });
    connect(ui_->betDisplay10Button, &QPushButton::clicked, this, [this]() { removeChip(10); });
    connect(ui_->betDisplay25Button, &QPushButton::clicked, this, [this]() { removeChip(25); });
    connect(ui_->betDisplay50Button, &QPushButton::clicked, this, [this]() { removeChip(50); });
    connect(ui_->betDisplay100Button, &QPushButton::clicked, this, [this]() { removeChip(100); });

    // TODO: change the placement of the connect calls below this comment
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

    // Show Count
    connect(ui_->showCountButton, &QPushButton::clicked, this, &GameWidget::displayCountingLabel);

    // Button presses.
    connect(ui_->hitButton, &QPushButton::clicked, game_, &BlackjackGame::playerHit);
    connect(ui_->standButton, &QPushButton::clicked, game_, &BlackjackGame::playerStand);
    connect(ui_->doubleButton, &QPushButton::clicked, game_, &BlackjackGame::playerDouble);
    connect(ui_->splitButton, &QPushButton::clicked, game_, &BlackjackGame::playerSplit);
    connect(ui_->returnButton, &QPushButton::clicked, this, &GameWidget::onReturnToMainMenu);

    // Tells QT to use the background color from the stylesheet
    setAttribute(Qt::WA_StyledBackground, true);
}

GameWidget::~GameWidget()
{
    delete ui_;
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
        message = "Blackjack";
        break;
    }

    // Add hand number if multiple hands
    if (totalHands > 1) {
        message = QString("Hand %1: %2").arg(handIndex + 1).arg(message);
    }

    // Update balance label.
    balance_ += payout;
    ui_->balanceLabel->setText("$" + QString::number(balance_));

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
                ui_->balanceLabel->setText("$" + QString::number(balance_));
                game_->setShuffling(true);
                GameWidget::resetGame();
            }
            else if (msgBox.clickedButton() == menuButton) {
                emit returnToMainMenu();
            }
        });
    }

    // Indicate results in bet label.
    ui_->betLabel->setVisible(true);
    ui_->betLabel->setText(message);

    // Make count label disappear for next round
    ui_->showCountLabel->setVisible(false);

    // Only reset game after the LAST hand is processed
    if (handIndex == totalHands - 1) {
        // Add extra delay after last hand before reset
        timer_.singleShot(2000, this, &GameWidget::resetGame);
    }
}

void GameWidget::beginBetStage() {
    // Show betting buttons
    ui_->startRoundButton->setVisible(true);
    ui_->startRoundButton->setEnabled(false);
    ui_->chip1Button->setVisible(true);
    ui_->chip5Button->setVisible(true);
    ui_->chip10Button->setVisible(true);
    ui_->chip25Button->setVisible(true);
    ui_->chip50Button->setVisible(true);
    ui_->chip100Button->setVisible(true);
    ui_->betAllButton->setVisible(true);
    setChipButtonsEnabled();

    // Hide bet text (without shifting layout)
    ui_->betLabel->setVisible(true);
    ui_->betLabel->setText("");

    // Hide gameplay buttons
    ui_->hitButton->setVisible(false);
    ui_->standButton->setVisible(false);
    ui_->doubleButton->setVisible(false);
    ui_->splitButton->setVisible(false);
    ui_->showCountButton->setVisible(false);
}

void GameWidget::addChip(int value) {
    // Update current bet
    currentBetTotal_ += value;
    currentBet_[value]++;

    // Enable "Start Round" button
    ui_->startRoundButton->setEnabled(true);

    // Add button to view so players can remove chip from bet and show count
    switch (value) {
    case 1:
        ui_->betDisplay1Button->setVisible(true);
        ui_->betDisplay1CountLabel->setVisible(true);
        ui_->betDisplay1CountLabel->setText("x" + QString::number(currentBet_[value]));
        break;
    case 5:
        ui_->betDisplay5Button->setVisible(true);
        ui_->betDisplay5CountLabel->setVisible(true);
        ui_->betDisplay5CountLabel->setText("x" + QString::number(currentBet_[value]));
        break;
    case 10:
        ui_->betDisplay10Button->setVisible(true);
        ui_->betDisplay10CountLabel->setVisible(true);
        ui_->betDisplay10CountLabel->setText("x" + QString::number(currentBet_[value]));
        break;
    case 25:
        ui_->betDisplay25Button->setVisible(true);
        ui_->betDisplay25CountLabel->setVisible(true);
        ui_->betDisplay25CountLabel->setText("x" + QString::number(currentBet_[value]));
        break;
    case 50:
        ui_->betDisplay50Button->setVisible(true);
        ui_->betDisplay50CountLabel->setVisible(true);
        ui_->betDisplay50CountLabel->setText("x" + QString::number(currentBet_[value]));
        break;
    case 100:
        ui_->betDisplay100Button->setVisible(true);
        ui_->betDisplay100CountLabel->setVisible(true);
        ui_->betDisplay100CountLabel->setText("x" + QString::number(currentBet_[value]));
        break;
    default:
        break;  // Should never run
    }

    // Update current bet label
    ui_->betLabel->setText("-$" + QString::number(currentBetTotal_));

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
            ui_->betDisplay1Button->setVisible(false);
            ui_->betDisplay1CountLabel->setVisible(false);
        } else {
            ui_->betDisplay1CountLabel->setText("x" + QString::number(newChipCount));
        }
        break;
    case 5:
        if (newChipCount == 0) {
            ui_->betDisplay5Button->setVisible(false);
            ui_->betDisplay5CountLabel->setVisible(false);
        } else {
            ui_->betDisplay5CountLabel->setText("x" + QString::number(newChipCount));
        }
        break;
    case 10:
        if (newChipCount == 0) {
            ui_->betDisplay10Button->setVisible(false);
            ui_->betDisplay10CountLabel->setVisible(false);
        } else {
            ui_->betDisplay10CountLabel->setText("x" + QString::number(newChipCount));
        }
        break;
    case 25:
        if (newChipCount == 0) {
            ui_->betDisplay25Button->setVisible(false);
            ui_->betDisplay25CountLabel->setVisible(false);
        } else {
            ui_->betDisplay25CountLabel->setText("x" + QString::number(newChipCount));
        }
        break;
    case 50:
        if (newChipCount == 0) {
            ui_->betDisplay50Button->setVisible(false);
            ui_->betDisplay50CountLabel->setVisible(false);
        } else {
            ui_->betDisplay50CountLabel->setText("x" + QString::number(newChipCount));
        }
        break;
    case 100:
        if (newChipCount == 0) {
            ui_->betDisplay100Button->setVisible(false);
            ui_->betDisplay100CountLabel->setVisible(false);
        } else {
            ui_->betDisplay100CountLabel->setText("x" + QString::number(newChipCount));
        }
        break;
    default:
        break;  // Should never run
    }

    // Update current bet label and disable start button if necessary
    if (currentBetTotal_ == 0) {
        ui_->betLabel->setText("");
        ui_->startRoundButton->setEnabled(false);
    }
    else
        ui_->betLabel->setText("-$" + QString::number(currentBetTotal_));

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
    ui_->startRoundButton->setVisible(false);

    // Gameplay buttons will be shown by onTurnChanged signal

    // remove betting buttons
    ui_->chip1Button->setVisible(false);
    ui_->chip5Button->setVisible(false);
    ui_->chip10Button->setVisible(false);
    ui_->chip25Button->setVisible(false);
    ui_->chip50Button->setVisible(false);
    ui_->chip100Button->setVisible(false);
    ui_->betAllButton->setVisible(false);

    // Hide bet label
    ui_->betLabel->setText("");

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
}

void GameWidget::onDealerCardDealt(Card card) {
    cardsView_->dealDealerCard(card);
}

void GameWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    // Resize cardsView
    cardsView_->setGeometry(rect());

    // Resize overlay if visible
    if (strategyOverlay_ && strategyOverlay_->isVisible()) {
        strategyOverlay_->setGeometry(rect());
    }
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
    ui_->betDisplay1Button->setVisible(false);
    ui_->betDisplay1CountLabel->setVisible(false);
    ui_->betDisplay5Button->setVisible(false);
    ui_->betDisplay5CountLabel->setVisible(false);
    ui_->betDisplay10Button->setVisible(false);
    ui_->betDisplay10CountLabel->setVisible(false);
    ui_->betDisplay25Button->setVisible(false);
    ui_->betDisplay25CountLabel->setVisible(false);
    ui_->betDisplay50Button->setVisible(false);
    ui_->betDisplay50CountLabel->setVisible(false);
    ui_->betDisplay100Button->setVisible(false);
    ui_->betDisplay100CountLabel->setVisible(false);

    beginBetStage();
}

void GameWidget::onPlayerTurn(int handIndex, bool canDouble, bool canSplit) {
    // Track which hand is active to support multi-hand logic
    currentHandIndex_ = handIndex;

    // Show gameplay buttons for player's turn
    ui_->hitButton->setVisible(true);
    ui_->standButton->setVisible(true);
    ui_->showCountButton->setVisible(true);

    // Show double/split buttons based on game logic decision
    ui_->doubleButton->setVisible(canDouble);
    ui_->splitButton->setVisible(canSplit);
}

void GameWidget::onDealerTurnStarted() {
    // Hide all gameplay buttons during dealer's turn
    ui_->hitButton->setVisible(false);
    ui_->standButton->setVisible(false);
    ui_->doubleButton->setVisible(false);
    ui_->splitButton->setVisible(false);

    // Disable chip buttons (but keep them visible)
    ui_->betDisplay1Button->setEnabled(false);
    ui_->betDisplay5Button->setEnabled(false);
    ui_->betDisplay10Button->setEnabled(false);
    ui_->betDisplay25Button->setEnabled(false);
    ui_->betDisplay50Button->setEnabled(false);
    ui_->betDisplay100Button->setEnabled(false);

    // Flip hole card
    cardsView_->flipDealerHoleCard();
}

void GameWidget::onHandSplit(int handIndex) {
    cardsView_->handleHandSplit(handIndex);
}

void GameWidget::onBetPlaced(int betAmount) {
    balance_ -= betAmount;
    ui_->betLabel->setText("-$" + QString::number(betAmount));
    timer_.singleShot(2000, [this]() {
        ui_->balanceLabel->setText("$" + QString::number(balance_));
        ui_->betLabel->setText("");
    });
}

void GameWidget::displayCountingLabel() {
    ui_->showCountLabel->setVisible(true);

    std::string runningCount = std::to_string(game_->getRunningCount());
    std::string trueCount = std::to_string(game_->getTrueCount());

    QString labelText = QString("Running Count: ") +
                        QString::fromStdString(runningCount) +
                        QString("\n") +
                        QString("True Count: ") +
                        QString::fromStdString(trueCount);

    ui_->showCountLabel->setText(labelText);
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

void GameWidget::onAllIn() {
    int remaining = balance_ - currentBetTotal_;
    if (remaining <= 0) {
        return;
    }

    const int chipValues[] = {100, 50, 25, 10, 5, 1};

    for (int value : chipValues) {
        while (remaining >= value) {
            addChip(value);
            remaining -= value;
        }
    }
}

void GameWidget::onStrategyChartButtonClicked() {
    strategyOverlay_->showOverlay();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game_widget.h"
#include "blackjack_game.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , currentRules_()
{
    ui_->setupUi(this);

    // Create the stacked widget
    stackedWidget_ = new QStackedWidget(this);

    // Store the menu widget (which is the current centralwidget from the ui_)
    menuWidget_ = ui_->centralwidget;

    // Add the menu widget to the stacked widget
    stackedWidget_->addWidget(menuWidget_);

    // Set the stacked widget as the central widget
    setCentralWidget(stackedWidget_);

    // Initialize and add rule set widget to stacked widget
    rulesetWidget_ = new RulesetWidget(this);
    stackedWidget_->addWidget(rulesetWidget_);

    // Initialize and add learn widget to stacked widget
    learnWidget_ = new LearnWidget(this);
    stackedWidget_->addWidget(learnWidget_);

    // Connect the practice button to the slot
    connect(ui_->practiceButton, &QPushButton::clicked, this, &MainWindow::onPracticeButtonClicked);

    // Rulset menu connections
    connect(ui_->rulesetButton, &QPushButton::clicked, this, &MainWindow::onRulesetButtonClicked);
    connect(rulesetWidget_, &RulesetWidget::returnToMainMenu, this, &MainWindow::onReturnToMainMenuClicked);
    connect(rulesetWidget_, &RulesetWidget::saveRulesRequested, this, &MainWindow::onRulesetSaved);

    // Learn connection
    connect(ui_->learnButton, &QPushButton::clicked, this, &MainWindow::onLearnButtonClicked);
    connect(learnWidget_, &LearnWidget::returnToMainMenu, this, &MainWindow::onReturnToMainMenuClicked);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::onLearnButtonClicked(){
    stackedWidget_->setCurrentIndex(2);
}

void MainWindow::onRulesetSaved() {
    // Retrieve the configuration from the widget and store it
    currentRules_ = rulesetWidget_->getRuleset();
}

void MainWindow::onReturnToMainMenuClicked(){
    stackedWidget_->setCurrentWidget(menuWidget_);
}

void MainWindow::onPracticeButtonClicked()
{
    // Create a new BlackjackGame
    BlackjackGame* game = new BlackjackGame(this);
    game->setRuleset(currentRules_);

    // Create a new GameWidget with the game
    GameWidget* gameWidget = new GameWidget(game, this);

    // Add the game widget to the stacked widget
    stackedWidget_->addWidget(gameWidget);

    // Switch to the game widget
    stackedWidget_->setCurrentWidget(gameWidget);

    connect(gameWidget, &GameWidget::returnToMainMenu, this, &MainWindow::onReturnToMainMenuClicked);

    // Start the betting stage
    gameWidget->beginBetStage();
}

void MainWindow::onRulesetButtonClicked() {
    // Load the current stored rules into the widget before showing it
    rulesetWidget_->setRuleset(currentRules_);
    stackedWidget_->setCurrentWidget(rulesetWidget_);
}

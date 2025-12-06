#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game_widget.h"
#include "blackjack_game.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
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

    // Initialize and add learn widget to stacked widget
    rulesetWidget_ = new RulesetWidget(this);
    stackedWidget_->addWidget(rulesetWidget_);

    // Connect the practice button to the slot
    connect(ui_->practiceButton, &QPushButton::clicked, this, &MainWindow::onPracticeButtonClicked);

    // Connect the rules button to the slot
    connect(ui_->rulesetButton, &QPushButton::clicked, this, &MainWindow::onRulesetButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::onPracticeButtonClicked()
{
    // Create a new BlackjackGame
    BlackjackGame* game = new BlackjackGame(this);

    // Create a new GameWidget with the game
    GameWidget* gameWidget = new GameWidget(game, this);

    // Add the game widget to the stacked widget
    stackedWidget_->addWidget(gameWidget);

    // Switch to the game widget
    stackedWidget_->setCurrentWidget(gameWidget);

    // Start the betting stage
    gameWidget->beginBetStage();
}

void MainWindow::onRulesetButtonClicked() {
    stackedWidget_->setCurrentWidget(rulesetWidget_);
}

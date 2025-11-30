#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "learn_widget.h"
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
    learnWidget_ = new LearnWidget(this);
    stackedWidget_->addWidget(learnWidget_);

    // Connect the practice button to the slot
    connect(ui_->practiceButton, &QPushButton::clicked, this, &MainWindow::onPracticeButtonClicked);

    // Connect the learn button to the slot
    connect(ui_->learnButton, &QPushButton::clicked, this, &MainWindow::onLearnButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::onPracticeButtonClicked(){

}

void MainWindow::onLearnButtonClicked(){
    stackedWidget_->setCurrentIndex(1);
}

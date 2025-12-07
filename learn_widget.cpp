#include "learn_widget.h"
#include "ui_learn_widget.h"

LearnWidget::LearnWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LearnWidget)
{
    ui->setupUi(this);
}

LearnWidget::~LearnWidget()
{
    delete ui;
}

#include "learn_widget.h"
#include "ui_learn_widget.h"

learn_widget::learn_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::learn_widget)
{
    ui->setupUi(this);
}

learn_widget::~learn_widget()
{
    delete ui;
}

#include "ruleset_widget.h"
#include "ui_ruleset_widget.h"

ruleset_widget::ruleset_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ruleset_widget)
{
    ui->setupUi(this);
}

ruleset_widget::~ruleset_widget()
{
    delete ui;
}

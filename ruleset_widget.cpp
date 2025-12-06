#include "ruleset_widget.h"
#include "ui_ruleset_widget.h"

RulesetWidget::RulesetWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ruleset_widget)
{
    ui->setupUi(this);
}

RulesetWidget::~RulesetWidget()
{
    delete ui;
}

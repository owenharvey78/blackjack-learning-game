#include "ruleset_widget.h"
#include "ui_ruleset_widget.h"

RulesetWidget::RulesetWidget(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::ruleset_widget) {
    ui_->setupUi(this);
}

RulesetWidget::~RulesetWidget() {
    delete ui_;
}

void RulesetWidget::setRuleset(const Ruleset& rules) {
    // Map Ruleset struct to UI
    ui_->checkBox->setChecked(rules.dealerHitsSoft17);
    ui_->checkBox_2->setChecked(rules.dealerPeeks);
    ui_->checkBox_3->setChecked(rules.pushOnDealer22);
    ui_->blackJackPayout->setValue(rules.blackjackPayout);
    ui_->numDecks->setValue(rules.numDecks);
    ui_->checkBox_4->setChecked(rules.doubleAfterSplit);
    ui_->checkBox_5->setChecked(rules.resplit);
    ui_->checkBox_6->setChecked(rules.hitSplitAces);
    ui_->checkBox_7->setChecked(rules.resplitAces);
    ui_->checkBox_8->setChecked(rules.surrenderAllowed);
}

Ruleset RulesetWidget::getRuleset() const {
    Ruleset rules;
    // Map UI to ruleset
    rules.dealerHitsSoft17 = ui_->checkBox->isChecked();
    rules.dealerPeeks = ui_->checkBox_2->isChecked();
    rules.pushOnDealer22 = ui_->checkBox_3->isChecked();
    rules.blackjackPayout = ui_->blackJackPayout->value();
    rules.numDecks = ui_->numDecks->value();
    rules.doubleAfterSplit = ui_->checkBox_4->isChecked();
    rules.resplit = ui_->checkBox_5->isChecked();
    rules.hitSplitAces = ui_->checkBox_6->isChecked();
    rules.resplitAces = ui_->checkBox_7->isChecked();
    rules.surrenderAllowed = ui_->checkBox_8->isChecked();
    return rules;
}

void RulesetWidget::on_saveButton_clicked() {
    emit saveRulesRequested(); // Notify MainWindow to save
}

void RulesetWidget::on_menuButton_clicked() {
    emit returnToMainMenu();   // Go back without saving
}



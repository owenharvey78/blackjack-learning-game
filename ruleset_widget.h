#ifndef RULESET_WIDGET_H
#define RULESET_WIDGET_H

#include <QWidget>
#include "ruleset.h" // Assuming your struct is here
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>

namespace Ui {
class ruleset_widget;
}

class ruleset_widget : public QWidget
{
    Q_OBJECT

public:
    explicit ruleset_widget(QWidget *parent = nullptr);
    ~ruleset_widget();

    Ruleset getRuleset() const;

    void setRuleset(const Ruleset& rules);

private slots:
    // we will need a slot for each rule that can be changed
    //    - some will be toggled while some will be input

signals:
    void rulesChanged(Ruleset newRules);

    // Dealer Rules Controls
    QCheckBox* dealerHitsSoft17Box_;
    QCheckBox* dealerPeeksBox_;
    QCheckBox* pushOnDealer22Box_;

    // Payouts/Decks Controls
    QDoubleSpinBox* blackjackPayoutSpin_;
    QSpinBox* numDecksSpin_;

    // Restrictions Controls
    QCheckBox* doubleAfterSplitBox_;
    QCheckBox* resplitBox_;
    QCheckBox* hitSplitAcesBox_;
    QCheckBox* resplitAcesBox_;
    QCheckBox* surrenderAllowedBox_;

private:
    Ui::ruleset_widget *ui;
};

#endif // RULESET_WIDGET_H

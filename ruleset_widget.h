#ifndef RULESET_WIDGET_H
#define RULESET_WIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>

namespace Ui {
class ruleset_widget;
}

class RulesetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RulesetWidget(QWidget *parent = nullptr);
    ~RulesetWidget();

    // Ruleset getRuleset() const;

    // void setRuleset(const Ruleset& rules);

private slots:
    // we will need a slot for each rule that can be changed
    //    - some will be toggled while some will be input

signals:
    // void rulesChanged(Ruleset newRules);

private:
    Ui::ruleset_widget *ui;
};

#endif // RULESET_WIDGET_H

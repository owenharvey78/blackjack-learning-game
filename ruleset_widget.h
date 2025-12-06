#ifndef RULESET_WIDGET_H
#define RULESET_WIDGET_H

#include <QWidget>
#include "ruleset.h" // Include your struct

namespace Ui {
class ruleset_widget;
}

class RulesetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RulesetWidget(QWidget *parent = nullptr);
    ~RulesetWidget();

    // Get the current state of UI as a Ruleset struct
    Ruleset getRuleset() const;

    // Update the UI to match an existing Ruleset
    void setRuleset(const Ruleset& rules);

signals:
    void returnToMainMenu();
    // Signal to tell MainWindow to grab the new rules
    void saveRulesRequested();

private slots:
    // Auto-connected slots for UI buttons
    void on_saveButton_clicked();
    void on_menuButton_clicked();

private:
    Ui::ruleset_widget *ui_;
};

#endif // RULESET_WIDGET_H

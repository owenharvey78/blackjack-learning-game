#ifndef RULESET_WIDGET_H
#define RULESET_WIDGET_H

#include <QWidget>
#include "ruleset.h"

namespace Ui {
class ruleset_widget;
}

/// @brief the widget defining the view for the rule selection menu.
class RulesetWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructs a new GameWidget.
    /// @param parent The parent widget of this widget.
    explicit RulesetWidget(QWidget *parent = nullptr);

    /// @brief Frees any resources associated with this GameWidget.
    ~RulesetWidget();

    /// @brief Gets the ruleset from the current configuration.
    Ruleset getRuleset() const;

    /// @brief Sets the UI to match the existing ruleset.
    void setRuleset(const Ruleset& rules);

signals:
    /// @brief Signals that the user has requested to return to the main menu.
    void returnToMainMenu();

    /// @brief Signals that the user has requested to save the rules.
    void saveRulesRequested();

private slots:
    /// @brief Emits the signal to return to the main menu.
    void on_menuButton_clicked();

    /// @brief Emits the signal to save the selected rules.
    void on_saveButton_clicked();

private:
    /// @brief The UI form associated with this widget.
    Ui::ruleset_widget *ui_;
};

#endif // RULESET_WIDGET_H

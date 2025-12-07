#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ruleset_widget.h"
#include <QMainWindow>
#include <QStackedWidget>
#include "ruleset.h"
#include "learn_widget.h"

class GameWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// @brief When the practice button is selected in the main menu,
    /// switches the central widget to game widget.
    void onPracticeButtonClicked();

    /// @brief When the ruleset button is selected in the main menu,
    /// switches the central widget to ruleset widget.
    void onRulesetButtonClicked();

    /// @brief When the main menu button is selected in the ruleset menu,
    /// switches the central widget to main menu widget.
    void onReturnToMainMenuClicked();

    /// @brief When the save button is selected in the ruleset menu,
    /// saves the current configuration as the new ruleset.
    void onRulesetSaved();

    /// @brief When the learn button is pressed the learn widget becomes the cental widget.
    void onLearnButtonClicked();

private:
    /// @brief The UI form object for this window.
    Ui::MainWindow* ui_;

    /// @brief A QStackedWidget used for ensuring the game takes complete control
    /// over the application when it starts.
    QStackedWidget* stackedWidget_;

    /// @brief The widget for the main menu.
    QWidget* menuWidget_;

    /// @brief The widget for the ruleset selection menu.
    RulesetWidget* rulesetWidget_;

    /// @brief The widget for the learn page
    LearnWidget* learnWidget_;

    /// @brief The current rules.
    Ruleset currentRules_;

};
#endif // MAINWINDOW_H

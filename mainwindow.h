#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ruleset_widget.h"
#include <QMainWindow>
#include <QStackedWidget>
#include "ruleset.h"

class GameWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPracticeButtonClicked();
    void onRulesetButtonClicked();
    void onReturnToMainMenuClicked();
    void onRulesetSaved();

private:
    /// @brief The UI form object for this window.
    Ui::MainWindow* ui_;

    /// @brief A QStackedWidget used for ensuring the game takes complete control
    /// over the application when it starts.
    QStackedWidget* stackedWidget_;

    /// @brief The widget for the main menu.
    QWidget* menuWidget_;

    RulesetWidget* rulesetWidget_;

    Ruleset currentRules_;

};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

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
    /// @brief Handles actions when practice button is clicked.
    void onPracticeButtonClicked();

    /// @brief Handles actions when returning from practice.
    void onFromPracticeReturnToMenu();

private:
    /// @brief The UI form object for this window.
    Ui::MainWindow* ui_;

    /// @brief A QStackedWidget used for ensuring the game takes complete control
    /// over the application when it starts.
    QStackedWidget* stackedWidget_;

    /// @brief The widget for the main menu.
    QWidget* menuWidget_;

    /// @brief The game widget.
    GameWidget* gameWidget_;
};
#endif // MAINWINDOW_H

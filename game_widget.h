#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private:
    Ui::GameWidget *ui;
};

#endif // GAME_WIDGET_H

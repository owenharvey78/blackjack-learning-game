#ifndef LEARN_WIDGET_H
#define LEARN_WIDGET_H

#include <QWidget>

namespace Ui {
class learn_widget;
}

class learn_widget : public QWidget
{
    Q_OBJECT

public:
    explicit learn_widget(QWidget *parent = nullptr);
    ~learn_widget();

private:
    Ui::learn_widget *ui;
};

#endif // LEARN_WIDGET_H

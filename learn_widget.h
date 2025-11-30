#ifndef LEARN_WIDGET_H
#define LEARN_WIDGET_H

#include <QWidget>

namespace Ui {
class learn_widget;
}

class LearnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LearnWidget(QWidget *parent = nullptr);
    ~LearnWidget();

private:
    Ui::learn_widget *ui;
};

#endif // LEARN_WIDGET_H

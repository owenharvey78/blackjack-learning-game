#ifndef LEARN_WIDGET_H
#define LEARN_WIDGET_H

#include <QWidget>

namespace Ui {
class LearnWidget;
}

class LearnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LearnWidget(QWidget *parent = nullptr);
    ~LearnWidget();

private:
    Ui::LearnWidget *ui;
};

#endif // LEARN_WIDGET_H

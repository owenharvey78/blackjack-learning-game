#ifndef RULESET_WIDGET_H
#define RULESET_WIDGET_H

#include <QWidget>

namespace Ui {
class ruleset_widget;
}

class ruleset_widget : public QWidget
{
    Q_OBJECT

public:
    explicit ruleset_widget(QWidget *parent = nullptr);
    ~ruleset_widget();

private:
    Ui::ruleset_widget *ui;
};

#endif // RULESET_WIDGET_H

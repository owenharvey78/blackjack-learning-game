#include "strategy_chart_dialog.h"
#include "ui_strategy_chart_dialog.h"
#include <QMouseEvent>
#include <QResizeEvent>

StrategyChartDialog::StrategyChartDialog(bool dealerHitsSoft17, QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::StrategyChartDialog)
{
    ui_->setupUi(this);

    QString imagePath = dealerHitsSoft17
        ? ":images/h17_strategy_chart.png"
        : ":images/s17_strategy_chart.png";
    originalPixmap_ = QPixmap(imagePath);

    // Tells QT to style the background using the style sheet in the UI form
    setAttribute(Qt::WA_StyledBackground, true);

    hide();
}

StrategyChartDialog::~StrategyChartDialog()
{
    delete ui_;
}

void StrategyChartDialog::showOverlay()
{
    if (parentWidget()) {
        setGeometry(parentWidget()->rect());
    }

    // Set chart size as 80% of dialog size (with slight extra margin)
    int maxWidth = width() * 0.8;
    int maxHeight = height() * 0.8;
    QPixmap scaledPixmap = originalPixmap_.scaled(
        maxWidth - 40,  // Add extra margins to outside of chart
        maxHeight - 40,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );
    ui_->chartLabel->setPixmap(scaledPixmap);
    int widgetWidth = scaledPixmap.width() + 40;
    int widgetHeight = scaledPixmap.height() + 40;
    ui_->chartWidget->setFixedSize(widgetWidth, widgetHeight);

    // Center the chart widget
    int x = (width() - widgetWidth) / 2;
    int y = (height() - widgetHeight) / 2;
    ui_->chartWidget->move(x, y);

    // Bring to front and show
    raise();
    show();
    setFocus();
}

void StrategyChartDialog::hideOverlay()
{
    hide();
}

void StrategyChartDialog::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (isVisible()) {
        showOverlay();  // Already resizes the content on its own
    }
}

void StrategyChartDialog::mousePressEvent(QMouseEvent* event)
{
    // Check if click is outside chart widget
    if (!ui_->chartWidget->geometry().contains(event->pos())) {
        hideOverlay();
    } else {
        QWidget::mousePressEvent(event);
    }
}

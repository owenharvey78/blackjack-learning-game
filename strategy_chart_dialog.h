#ifndef STRATEGY_CHART_DIALOG_H
#define STRATEGY_CHART_DIALOG_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class StrategyChartDialog;
}

/// @brief An overlay widget for showing the basic strategy chart.
class StrategyChartDialog : public QWidget {
    Q_OBJECT

public:
    /// @brief Constructs a new StrategyChartDialog.
    /// @param dealerHitsSoft17 Indicates whether to show the H17 basic strategy
    /// chart (true) or the S17 chart (false).
    /// @param parent The parent widget (should be GameWidget)
    explicit StrategyChartDialog(bool dealerHitsSoft17, QWidget* parent = nullptr);
    ~StrategyChartDialog();

    /// @brief Shows the overlay and positions it on top of the parent widget.
    void showOverlay();

    /// @brief Hides the overlay.
    void hideOverlay();

protected:
    /// @brief Keeps the chart overlay properly positioned when resizing.
    void resizeEvent(QResizeEvent* event) override;

    /// @brief Enables the user to click outside the chart to close the dialog.
    void mousePressEvent(QMouseEvent* event) override;

private:
    /// @brief The UI form for the StrategyChartDialog.
    Ui::StrategyChartDialog* ui_;

    /// @brief The basic strategy chart image before any scaling is applied.
    QPixmap originalPixmap_;
};

#endif // STRATEGY_CHART_DIALOG_H
